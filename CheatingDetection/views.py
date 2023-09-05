from django.shortcuts import render, redirect
from django.core.files.storage import default_storage
from django.conf import settings
from django.http import Http404
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


def result_view(request):
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

    result_info = []
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

        result_info.append({
            "score": score,
            "id": id,
            "name": name,
            "language": language,
            "path": info,
        })

    context = {"result": result_info}

    return render(request, 'result.html', context)


def run_code():
    os.chdir('data')
    executable_path = './CheatingDetection'
    result = subprocess.run([executable_path], capture_output=True, text=True)
    os.chdir('..')
    return result


def submission_view(request, file_name):
    try:
        with open('data/Submission/' + file_name, 'r') as file:
            file_source = file.read()
            context = {"code": file_source}
    except FileNotFoundError:
        raise Http404("Invalid URL")
    return render(request, 'submission.html', context)
