import time
from bs4 import BeautifulSoup
import json
import requests
from urllib import parse
import re
import random
import os

script_dir = os.path.dirname(os.path.abspath(__file__)) # 获取脚本所在的目录
session_id_file = os.path.join(script_dir, "session_id") # 构造文件的完整路径
program_file = os.path.join(script_dir, "main.c") # 构造文件的完整路径


def get_upload_endpoint(session, url):
    # https://judge.buaa.edu.cn/assignment/programList.jsp?proNum=1&assignID=1387
    parsed_url = parse.urlparse(url)
    query_dict = parse.parse_qs(parsed_url.query)
    r = session.get(url, headers={"Referer": "https://judge.buaa.edu.cn/assignment/index.jsp?assignID={}".format(query_dict["assignID"][0])})
    if r.status_code != 200:
        print("Invalid status code: {} from: {}".format(r.status_code, url))
        return None
    else:
        response_body = r.text
        m = re.search(r'document\.upload\.action = "(.*?)";', response_body)
        if m is None:
            print("Cannot find upload endpoint from: {}".format(url))
            print("response: {}".format(r.text))
        else:
            upload_endpoint = "https://judge.buaa.edu.cn/assignment/" + m.group(1)
            # print("upload endpoint: {}".format(upload_endpoint))
            return upload_endpoint

def upload_file(session, base_url, endpoint_url, file):
    file_base, file_ext = file.split(".")
    if file_ext not in ["c", "zip"]:
        print("Invalid file extension: {} for {}".format(file_ext, file))
        exit(1)
    with open(file, "rb") as f:
        if file_ext == "c":
            files = {"FILE1": (file, f, "text/x-csrc")}
        else:
            files = {"FILE1": (file, f, "application/zip")}
        r = session.post(endpoint_url, files=files, headers={"Referer": base_url})
        if r.status_code != 200:
            print("Invalid status code: {} from: {}".format(r.status_code, endpoint_url))
            print("response: {}".format(r.text))
        else:
            response_body = r.text
            m = re.search(r'ajaxGetURL\("result", "(.*?)", cb\);', response_body)
            if m is None:
                print("Cannot find result url from: {}".format(endpoint_url))
                print("response: {}".format(r.text))
            else:
                result_url = "https://judge.buaa.edu.cn" + m.group(1)
                # print("result url: {}".format(result_url))
                return result_url

def get_result(session, result_url):
    while True:
        r = session.get(result_url)
        if r.status_code != 200:
            print("Invalid status code: {} from: {}".format(r.status_code, result_url))
            print("response: {}".format(r.text))
        else:
            response_body = r.text
            if "正在评判......，请稍等" in response_body:
                #print("Judging...")
                time.sleep(1)
            else:
                if "编译错误" in response_body:
                    print("Compile error")
                    break
                elif "平均CPU时间" in response_body:
                    m = re.search(r"平均CPU时间:(.*?)S", response_body)
                    cpu_time = float(m.group(1))

                    result_html = json.loads(response_body)[1]["content"]
                    soup = BeautifulSoup(result_html, "html.parser")
                    trs = soup.find_all("tr")
                    print("cpu time: {:.3f}ms".format(cpu_time * 1000))
                    accepted = True
                    for tr in trs:
                        tds = tr.find_all("td")
                        if tds[1].text != "评判结果":
                            if tds[1].text != "完全正确":
                                accepted = False
                            #print(tds[0].text, tds[1].text)

                    return accepted, cpu_time * 1000

with open(session_id_file, "r") as f:
    session_id = f.readline().strip()

target_time = float(input("Please input the target time(ms): "))
s = requests.session()

s.cookies.set("JSESSIONID", session_id)
s.headers.update({"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:126.0) Gecko/20100101 Firefox/126.0"})
s.headers.update({"Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"})
s.headers.update({"Accept-Language": "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2"})
s.headers.update({"Accept-Encoding": "gzip, deflate, br, zstd"})
s.headers.update({"Origin": "https://judge.buaa.edu.cn"})
s.headers.update({"Sec-Fetch-Dest": "iframe"})
s.headers.update({"Sec-Fetch-Mode": "navigate"})
s.headers.update({"Sec-Fetch-Site": "same-origin"})
s.headers.update({"Sec-Fetch-User": "?1"})
s.headers.update({"Sec-GPC": "1"})
count = 0

cpu_time_list = []

base_url = "https://judge.buaa.edu.cn/assignment/programList.jsp?proNum=2&courseID=25&assignID=1387&libCenter=false"

endpoint_url = get_upload_endpoint(s, base_url)

while True:
    print("count: {}".format(count))

    result_url = upload_file(s, base_url, endpoint_url, program_file)

    accepted, cpu_time = get_result(s, result_url)

    cpu_time_list.append(cpu_time)
    if cpu_time <= target_time:
        break
    if count >= 400:
        break
    time.sleep(random.randrange(3, 10) / 10)
    count += 1
print(cpu_time_list)