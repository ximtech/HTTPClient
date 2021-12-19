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

typedef struct HTTP HTTP;

typedef struct GET {
    struct GET (*bindParam)(const char *key, char *value);
    struct GET (*addHeader)(HTTPHeaderKey key, char *value);
    struct GET (*addCustomHeader)(const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTP *http);    // reactive mode: no polling wait for response
    HTTPResponse (*execute)(HTTP *http);            // enable busy wait for response
} GET;

typedef struct POST {
    struct POST (*bindParam)(char *key, char *value);
    struct POST (*bindJson)(const char *jsonString);
    struct POST (*addHeader)(HTTPHeaderKey key, char *value);
    struct POST (*addCustomHeader)(const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTP *http);
    HTTPResponse (*execute)(HTTP *http);
} POST;

typedef struct PUT {
    struct PUT (*addHeader)(HTTPHeaderKey key, char *value);
    struct PUT (*addCustomHeader)(const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTP *http);
    HTTPResponse (*execute)(HTTP *http);
} PUT;

typedef struct DELETE {
    struct DELETE (*addHeader)(HTTPHeaderKey key, char *value);
    struct DELETE (*addCustomHeader)(const char *key,  char *value);
    HTTPResponse (*executeNonBlock)(HTTP *http);
    HTTPResponse (*execute)(HTTP *http);
} DELETE;

typedef struct HEAD {
    struct HEAD (*addHeader)(HTTPHeaderKey key, char *value);
    struct HEAD (*addCustomHeader)(const char *key, char *value);
    HTTPResponse (*executeNonBlock)(HTTP *http);
    HTTPResponse (*execute)(HTTP *http);
} HEAD;

struct HTTP {
    struct GET (*GET)(char *host);
    struct POST (*POST)(char *host);
    struct PUT (*PUT)(char *host);
    struct DELETE (*DELETE)(char *host);
    struct HEAD (*HEAD)(char *host);

    char *requestDataBuffer;
    uint32_t requestBufferSize;
    HTTPResponse (*sendRequestCallback)(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute);
};

HTTP initHTTPInstance(char *dataBufferPointer, uint32_t size);
void registerHttpCallback(HTTP *http, HTTPResponse (*callbackFunction)(URLParser *, const char *, uint32_t, bool));// abstract function, must be implemented at the module inherited side
void deleteHttpClient();