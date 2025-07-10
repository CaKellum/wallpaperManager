#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/urlapi.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

const string DEFAULT_WALLPAPERP_CONFIG_PATH{"/.config/wallpaper.conf"};
const string DEFAULT_IMAGE_FOLDER{"/.wallpapers"};

struct RecivedFile {
  string path;
  FILE *stream;
};

static size_t write_response(void *buffer, size_t size, size_t nmemb,
                             void *userp);
static CURL *setup(string url, RecivedFile *file);
string filename_from(const string url);
vector<string> get_current_papers();
vector<string> get_urls_from(ifstream *file,
                             const vector<string> current_wallpapers);
ifstream get_file_at(string path);
vector<string> get_ulrs();
bool confirm_valid_state();
CURLcode get_paper_for(const string url);
string localize_path(string relative_path);

int main() {
  if (!confirm_valid_state()) {
    return 1;
  }
  vector<string> urls_string = get_ulrs();
  if (urls_string.empty()) {
    return 0; // no new papers
  }

  // making the requests loop
  string urlS = urls_string.at(0);
  for (string url : urls_string) {
    CURLcode res = get_paper_for(url);
    if (res != CURLE_OK) {
      cerr << curl_easy_strerror(res) << endl;
    }
  }
  return 0;
}

string localize_path(string relative_path) {
  string home{getenv("HOME")};
  return home + relative_path;
}

bool confirm_valid_state() {
  filesystem::path config_path{localize_path(DEFAULT_WALLPAPERP_CONFIG_PATH)};
  filesystem::directory_entry config_file{config_path};
  filesystem::path wallpapers_path{localize_path(DEFAULT_IMAGE_FOLDER)};
  if (!filesystem::exists(wallpapers_path)) {
    filesystem::create_directory(wallpapers_path);
  }
  return config_file.exists();
}

CURLcode get_paper_for(const string url) {
  RecivedFile file{localize_path(DEFAULT_IMAGE_FOLDER) + filename_from(url),
                   NULL};
  CURLcode res;
  CURL *url_real = setup(url, &file);
  res = curl_easy_perform(url_real);
  curl_easy_cleanup(url_real);
  if (file.stream) {
    fclose(file.stream);
  }
  return res;
}

vector<string> get_ulrs() {
  ifstream file = get_file_at(localize_path(DEFAULT_WALLPAPERP_CONFIG_PATH));
  if (!file.is_open()) {
    return {};
  }
  vector<string> current_wallpapers = get_current_papers();
  vector<string> urls_strings = get_urls_from(&file, current_wallpapers);
  file.close();
  return urls_strings;
}

ifstream get_file_at(string path) { return ifstream{path}; }

vector<string> get_urls_from(ifstream *file,
                             const vector<string> current_wallpapers) {
  vector<string> urls = {};
  string line;
  while (getline(*file, line)) {
    bool contains_paper =
        find(current_wallpapers.begin(), current_wallpapers.end(),
             filename_from(line)) != current_wallpapers.end();
    if (current_wallpapers.empty() || !contains_paper) {
      urls.push_back(line);
    }
  }
  return urls;
}

vector<string> get_current_papers() {
  auto papers = filesystem::directory_iterator{
      filesystem::path{localize_path(DEFAULT_IMAGE_FOLDER)}};
  vector<string> current_wallpapers = {};
  for (auto paper : papers) {
    current_wallpapers.push_back(paper.path().filename().string());
  }
  return current_wallpapers;
}

string filename_from(const string url) {
  size_t inx = url.rfind("/");
  if (inx < url.size()) {
    string name = url.substr(inx + 1);
    return name;
  }
  return url;
}

static CURL *setup(string url, RecivedFile *file) {
  CURL *handle = curl_easy_init();
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_response);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
  return handle;
}

static size_t write_response(void *buffer, size_t size, size_t nmemb,
                             void *userp) {
  RecivedFile *out = (RecivedFile *)userp;
  if (!out->stream) {
    out->stream = fopen(out->path.c_str(), "wb");
    if (!out->stream) {
      return 0;
    }
  }
  size_t written = fwrite(buffer, size, nmemb, out->stream);
  return written;
}
