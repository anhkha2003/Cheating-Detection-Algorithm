from django.shortcuts import render, redirect
from django.core.files.storage import default_storage
from django.conf import settings
from django.http import Http404
from .models import User, Submission
import os
import zipfile
import shutil
import subprocess

def get_files(path):
    names = []
    for root, dirs, files in os.walk(path):
        for file in files:
            full_path = os.path.join(root, file)
            relative_path = full_path[len(path)+1:] 
            names.append(relative_path)
    names.sort()
    return names


def home_view(request):
    code_names = get_files(settings.CODE_PATH)
    data_names = get_files(settings.TEST_DATA_PATH)    
    context = {
        "data_files": data_names,
        "code_files": code_names,
    }
    return render(request, 'home.html', context)


def store_and_unzip_file(file, path, folder_path):
    if os.path.isfile(path):
        os.remove(path)
    default_storage.save(path, file)

    if os.path.isdir(folder_path):
        shutil.rmtree(folder_path)
    with zipfile.ZipFile(path, 'r') as zip_ref:
        zip_ref.extractall(folder_path)


def process_view(request):
    data_path = 'data/data.zip'
    code_path = 'data/code.zip'

    if request.FILES.get('data'):
        folder_data_path = settings.TEST_DATA_PATH
        store_and_unzip_file(request.FILES.get('data'), data_path, folder_data_path)

    if request.FILES.get('code'):   
        folder_code_path = 'data/Submission'
        store_and_unzip_file(request.FILES.get('code'), code_path, folder_code_path)

    result = run_code()
    result_stdout = result.stdout.split('\n')

    submissions = []

    for line in result_stdout:
        content = line.split()
        if len(content) == 0:
            continue
        score, path = content

        info = os.path.basename(path)[:-1]
        language = info.split('.')[1]

        prefix = info.split('.')[0]
        underscore = prefix.find('_')
        id = prefix[:underscore]
        name = prefix[underscore+1:]

        with open('data/Submission/' + info, 'r') as file:
            source = file.read()

        user, _ = User.objects.get_or_create(name=name)

        submissions.append(Submission(
            sub_id=id,
            score=score,
            user=user,
            language=language,
            source=source,
        ))

    Submission.objects.bulk_create(
        submissions,
        update_conflicts=True,
        unique_fields=['sub_id'],
        update_fields=['score', 'user', 'language', 'source'],
    )
    return redirect('submissions')


def result_view(request):
    order = request.GET.get('order')
    queryset = Submission.objects
    if order:
        queryset = queryset.order_by(order)
    else:
        queryset = queryset.order_by('-score')

    page = int(request.GET.get('page', 1))
    queryset = queryset[(page - 1) * 50 : page * 50]
    context = {"result": queryset, "order": order}
    return render(request, 'result.html', context)


def run_code():
    os.chdir('data')
    executable_path = './CheatingDetection'
    result = subprocess.run([executable_path], capture_output=True, text=True)
    os.chdir('..')
    return result


def submission_view(request, sub_id):
    try:
        submission = Submission.objects.get(sub_id=sub_id)
        context = {"submission": submission}
    except Submission.DoesNotExist:
        raise Http404("Invalid URL")
    return render(request, 'submission.html', context)


def about_view(request):
    return render(request, 'about.html')