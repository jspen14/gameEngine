#include <curl/curl.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define PORT 3000
#define GET "GET"
#define POST "POST"

using namespace std;
string *DownloadedResponse;

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer_in)
{

    // Is there anything in the buffer?  
    if (buffer_in != NULL)  
    {
        // Append the data to the buffer    
        buffer_in->append(data, size * nmemb);

        // How much did we write?   
        DownloadedResponse = buffer_in;

        return size * nmemb;  
    }

    return 0;

}   
// <host>  <endpoint>  <GET/POST>
void makePostRequest(string host, string endpoint, string jsonObj ){

  CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    string completePath=host+endpoint;

    
    curl_easy_setopt(curl, CURLOPT_URL, completePath.c_str());
    /* Now specify the POST data */ 
 
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, POST);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj.c_str());
    curl_easy_setopt(curl, CURLOPT_PORT, PORT);
    
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  
}
string makeGetRequest(string host, string endpoint)
{
  CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    string completePath=host+endpoint;

    
    curl_easy_setopt(curl, CURLOPT_URL, completePath.c_str());
    /* Now specify the POST data */ 
 
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, GET);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writer);
    curl_easy_setopt(curl, CURLOPT_PORT, PORT);
    
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    
    /* Check for errors */ 
    if (CURLE_OK == res) 
        { 
            char *ct;         
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if((CURLE_OK == res) && ct)
                return *DownloadedResponse;
        }
    else
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return "";
}
void login(string host,string name, string password){
    string endpoint="/api/login";
    string jsonObj = "{ \"name\" : \""+name+"\" , \"password\" : \""+password+"\" }";
    makePostRequest(host, endpoint, jsonObj);
}
string getGameModels(string host)
{
  string endpoint="/api/gameModels";
  return makeGetRequest(host,endpoint);
}
int main(void)
{
    string userID=NULL;
    string host="10.24.67.92";
    //login(host, "capn","capn");
    getGameModels(host);
    return 0;
}

