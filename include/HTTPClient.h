#pragma once

#include "HTTPHeader.h"
#include "HTTPStatus.h"
#include "HTTPMethod.h"
#include "URLParser.h"
#include "HashMap.h"

#define HTTP_VERSION_HEADER  "HTTP/1.1"

typedef struct HTTPResponse {
    HTTPStatus statusCode;
	char *rawResponse;
    char *body;
} HTTPResponse;

typedef struct HTTPClient HTTPClient;

typedef struct GET {
    struct GET (*bindParam)(HTTPClient *client, const char *key, char *value);
    struct GET (*addHeader)(HTTPClient *client, HTTPHeaderKey key, char *value);
    struct GET (*addCustomHeader)(HTTPClient *client, const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTPClient *http);    // reactive mode: no polling wait for response
    HTTPResponse (*execute)(HTTPClient *http);            // enable busy wait for response
} GET;

typedef struct POST {
    struct POST (*bindParam)(HTTPClient *client, char *key, char *value);
    struct POST (*bindJson)(HTTPClient *client, const char *jsonString);
    struct POST (*addHeader)(HTTPClient *client, HTTPHeaderKey key, char *value);
    struct POST (*addCustomHeader)(HTTPClient *client, const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTPClient *client);
    HTTPResponse (*execute)(HTTPClient *client);
} POST;

typedef struct PUT {
    struct PUT (*addHeader)(HTTPClient *client, HTTPHeaderKey key, char *value);
    struct PUT (*addCustomHeader)(HTTPClient *client, const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTPClient *client);
    HTTPResponse (*execute)(HTTPClient *client);
} PUT;

typedef struct DELETE {
    struct DELETE (*addHeader)(HTTPClient *client, HTTPHeaderKey key, char *value);
    struct DELETE (*addCustomHeader)(HTTPClient *client, const char *key,  char *value);
    HTTPResponse (*executeNonBlock)(HTTPClient *client);
    HTTPResponse (*execute)(HTTPClient *client);
} DELETE;

typedef struct HEAD {
    struct HEAD (*addHeader)(HTTPClient *client, HTTPHeaderKey key, char *value);
    struct HEAD (*addCustomHeader)(HTTPClient *client, const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTPClient *client);
    HTTPResponse (*execute)(HTTPClient *client);
} HEAD;

struct HTTPClient {
    struct GET (*GET)(HTTPClient *client, char *host);
    struct POST (*POST)(HTTPClient *client, char *host);
    struct PUT (*PUT)(HTTPClient *client, char *host);
    struct DELETE (*DELETE)(HTTPClient *client, char *host);
    struct HEAD (*HEAD)(HTTPClient *client, char *host);

    URLParser url;
    HashMap headers;
    HashMap queryParameters;

    char *requestBuffer;
    uint32_t requestBufferSize;
    HTTPResponse (*sendRequestCallback)(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute);
};

void initHTTPClient(HTTPClient *client, char *dataBuffer, uint32_t size);
void registerHttpCallback(HTTPClient *client, HTTPResponse (*callbackFunction)(HTTPClient *, uint32_t, bool));// abstract function, must be implemented at the module inherited side
void deleteHttpClient(HTTPClient *client);