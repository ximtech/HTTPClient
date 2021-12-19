#include "HTTPClient.h"

#define HEADER_MAP_INITIAL_CAPACITY 4
#define QUERY_PARAMETER_MAP_INITIAL_CAPACITY 4
#define HEADER_SEMICOLON ": "
#define NEW_LINE         "\r\n"

static GET get(char *url);
static GET bindGetParam(const char *key, char *value);
static GET addGetHeader(HTTPHeaderKey key, char *value);
static GET addGetCustomHeader(const char *key, char *value);
static HTTPResponse executeGet(HTTP *http);
static HTTPResponse executeGetNonBlock(HTTP *http);

static POST post(char *rawUrl);
static POST bindPostParam(char *key, char *value);
static POST bindJson(const char *jsonString);
static POST addPostHeader(HTTPHeaderKey key, char *value);
static POST addPostCustomHeader(const char *key, char *value);
static HTTPResponse executePost(HTTP *http);
static HTTPResponse executePostNonBlock(HTTP *http);

static PUT put(char *rawUrl);
static PUT addPutHeader(HTTPHeaderKey key, char *value);
static PUT addPutCustomHeader(const char *key, char *value);
static HTTPResponse executePut(HTTP *http);
static HTTPResponse executePutNonBlock(HTTP *http);

static DELETE aDelete(char *rawUrl);
static DELETE addDeleteHeader(HTTPHeaderKey key, char *value);
static DELETE addDeleteCustomHeader(const char *key, char *value);
static HTTPResponse executeDelete(HTTP *http);
static HTTPResponse executeDeleteNonBlock(HTTP *http);

static HEAD head(char *rawUrl);
static HEAD addHeadHeader(HTTPHeaderKey key, char *value);
static HEAD addHeadCustomHeader(const char *key, char *value);
static HTTPResponse executeHead(HTTP *http);
static HTTPResponse executeHeadNonBlock(HTTP *http);

static void resetInnerData();
static void bindParam(const char *key, char *value);
static void addHeader(HTTPHeaderKey key, char *value);
static void addCustomHeader(const char *key, char *value);
static HTTPResponse doExecute(HTTPMethod method, HTTP *http, bool isBlockingExecute);

static uint32_t getUrlEncodedParamsLength();
static uint32_t getBodyEncodedParamsLength();
static uint32_t countParameterLength();
static uint32_t getHeadersLength();
static void addHeadersToRequest(char *requestBuffer);
static void addUrlEncodedParamsToRequest(char *requestBuffer);
static void addBodyEncodedParamsToRequest(char *requestBuffer);

static URLParser url;
static HashMap headerMap = NULL;
static HashMap queryParameterMap = NULL;

static GET httpGet = {NULL, NULL, NULL, NULL};
static POST httpPost = {NULL, NULL, NULL, NULL, NULL};
static PUT httpPut = {NULL, NULL, NULL};
static DELETE httpDelete = {NULL, NULL, NULL};
static HEAD httpHead = {NULL, NULL, NULL};


HTTP initHTTPInstance(char *dataBufferPointer, uint32_t size) {
    HTTP http = {NULL, NULL, NULL, NULL, NULL};

    http.GET = get;
    httpGet.bindParam = bindGetParam;
    httpGet.addHeader = addGetHeader;
    httpGet.addCustomHeader = addGetCustomHeader;
    httpGet.executeNonBlock = executeGetNonBlock;
    httpGet.execute = executeGet;

    http.POST = post;
    httpPost.bindParam = bindPostParam;
    httpPost.bindJson = bindJson;
    httpPost.addHeader = addPostHeader;
    httpPost.addCustomHeader = addPostCustomHeader;
    httpPost.executeNonBlock = executePostNonBlock;
    httpPost.execute = executePost;

    http.PUT = put;
    httpPut.addHeader = addPutHeader;
    httpPut.addCustomHeader = addPutCustomHeader;
    httpPut.executeNonBlock = executePutNonBlock;
    httpPut.execute = executePut;

    http.DELETE = aDelete;
    httpDelete.addHeader = addDeleteHeader;
    httpDelete.addCustomHeader = addDeleteCustomHeader;
    httpDelete.executeNonBlock = executeDeleteNonBlock;
    httpDelete.execute = executeDelete;

    http.HEAD = head;
    httpHead.addHeader = addHeadHeader;
    httpHead.addCustomHeader = addHeadCustomHeader;
    httpHead.executeNonBlock = executeHeadNonBlock;
    httpHead.execute = executeHead;

    http.requestDataBuffer = dataBufferPointer;
    http.requestBufferSize = size;

    initSingletonHashMap(&headerMap, HEADER_MAP_INITIAL_CAPACITY);
    initSingletonHashMap(&queryParameterMap, QUERY_PARAMETER_MAP_INITIAL_CAPACITY);
    return http;
}

void registerHttpCallback(HTTP *http, HTTPResponse (*callbackFunction)(URLParser *, const char *, uint32_t, bool)) {
    if (http != NULL && callbackFunction != NULL) {
        http->sendRequestCallback = callbackFunction;
    }
}

void deleteHttpClient() {
    hashMapDelete(headerMap);
    hashMapDelete(queryParameterMap);
    headerMap = NULL;
    queryParameterMap = NULL;
}

static GET get(char *rawUrl) {
    resetInnerData();
    parseUrlString(&url, rawUrl);
    return httpGet;
}

static GET bindGetParam(const char *key, char *value) {
    bindParam(key, value);
    return httpGet;
}

static GET addGetHeader(HTTPHeaderKey key, char *value) {
    addHeader(key, value);
    return httpGet;
}

static GET addGetCustomHeader(const char *key, char *value) {
    addCustomHeader(key, value);
    return httpGet;
}

static HTTPResponse executeGet(HTTP *http) {
    return doExecute(HTTP_GET, http, true);
}

static HTTPResponse executeGetNonBlock(HTTP *http) {
    return doExecute(HTTP_GET, http, false);
}


static POST post(char *rawUrl) {
    resetInnerData();
    parseUrlString(&url, rawUrl);
    return httpPost;
}

static POST bindPostParam(char *key, char *value) {
    bindParam(key, value);
    return httpPost;
}

static POST bindJson(const char *jsonString) {
    hashMapPut(queryParameterMap, "", (char *) jsonString);
    return httpPost;
}

static POST addPostHeader(HTTPHeaderKey key, char *value) {
    addHeader(key, value);
    return httpPost;
}

static POST addPostCustomHeader(const char *key, char *value) {
    addCustomHeader(key, value);
    return httpPost;
}

static HTTPResponse executePost(HTTP *http) {
    return doExecute(HTTP_POST, http, true);
}

static HTTPResponse executePostNonBlock(HTTP *http) {
    return doExecute(HTTP_POST, http, false);
}


static PUT put(char *rawUrl) {
    resetInnerData();
    parseUrlString(&url, rawUrl);
    return httpPut;
}

static PUT addPutHeader(HTTPHeaderKey key, char *value) {
    addHeader(key, value);
    return httpPut;
}

static PUT addPutCustomHeader(const char *key, char *value) {
    addCustomHeader(key, value);
    return httpPut;
}

static HTTPResponse executePut(HTTP *http) {
    return doExecute(HTTP_PUT, http, true);
}

static HTTPResponse executePutNonBlock(HTTP *http) {
    return doExecute(HTTP_PUT, http, false);
}


static DELETE aDelete(char *rawUrl) {
    resetInnerData();
    parseUrlString(&url, rawUrl);
    return httpDelete;
}

static DELETE addDeleteHeader(HTTPHeaderKey key, char *value) {
    addHeader(key, value);
    return httpDelete;
}

static DELETE addDeleteCustomHeader(const char *key, char *value) {
    addCustomHeader(key, value);
    return httpDelete;
}

static HTTPResponse executeDelete(HTTP *http) {
    return doExecute(HTTP_DELETE, http, true);
}

static HTTPResponse executeDeleteNonBlock(HTTP *http) {
    return doExecute(HTTP_DELETE, http, false);
}


static HEAD head(char *rawUrl) {
    resetInnerData();
    parseUrlString(&url, rawUrl);
    return httpHead;
}

static HEAD addHeadHeader(HTTPHeaderKey key, char *value) {
    addHeader(key, value);
    return httpHead;
}

static HEAD addHeadCustomHeader(const char *key, char *value) {
    addCustomHeader(key, value);
    return httpHead;
}

static HTTPResponse executeHead(HTTP *http) {
    return doExecute(HTTP_HEAD, http, true);
}

static HTTPResponse executeHeadNonBlock(HTTP *http) {
    return doExecute(HTTP_HEAD, http, false);
}


static void resetInnerData() {
    hashMapClear(queryParameterMap);
    hashMapClear(headerMap);
}

static void bindParam(const char *key, char *value) {
    if (isStringNotBlank(key) && isStringNotBlank(value)) {
        hashMapPut(queryParameterMap, key, value);
    }
}

static void addHeader(HTTPHeaderKey key, char *value) {
    addCustomHeader(getHeaderValueByKey(key), value);
}

static void addCustomHeader(const char *key, char *value) {
    if (isStringNotBlank(key) && isStringNotBlank(value)) {
        hashMapPut(headerMap, key, value);
    }
}

static HTTPResponse doExecute(HTTPMethod method, HTTP *http, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_NO_STATUS, NULL, NULL};
    if (url.isUrlValid && http->requestDataBuffer != NULL && http->requestBufferSize > 0) {
        uint32_t requestDataLength = 0;
        const char *methodName = getHttpMethodName(method);
        addHeader(HOST, url.host);  // add required header for HTTP/1.1

        requestDataLength += strlen(methodName);
        requestDataLength += strlen(" /");
        requestDataLength += strlen(url.path);

        if (method == HTTP_GET) {
            requestDataLength += getUrlEncodedParamsLength();
            requestDataLength += isStringNotEmpty(url.parameters) ? strlen(url.parameters) + 1 : 0; // +1 for delimiter "&" or "?"
        } else if (method == HTTP_POST) {
            uint32_t parametersLength = getBodyEncodedParamsLength();
            if (parametersLength > 0) {
                char dataLengthBuffer[sizeof(uint32_t) * 8 + 1];    // u32 full size and +1 for line end
                sprintf(dataLengthBuffer, "%lu", parametersLength);
                addHeader(CONTENT_LENGTH, dataLengthBuffer);
            }
            requestDataLength += parametersLength;
        }

        requestDataLength += strlen(" ");
        requestDataLength += strlen(HTTP_VERSION_HEADER);
        requestDataLength += strlen(NEW_LINE);
        requestDataLength += getHeadersLength();
        requestDataLength++;     // place for line end

        if (requestDataLength >= http->requestBufferSize) { // check that data fits buffer size
            return httpResponse;
        }
        memset(http->requestDataBuffer, 0, http->requestBufferSize);
        strcat(http->requestDataBuffer, methodName);
        strcat(http->requestDataBuffer, " /");
        strcat(http->requestDataBuffer, url.path);

        if (method == HTTP_GET) {
            addUrlEncodedParamsToRequest(http->requestDataBuffer);
            if (isStringNotEmpty(url.parameters)) {
                strcat(http->requestDataBuffer, isHashMapEmpty(queryParameterMap) ? "?" : "&");
                strcat(http->requestDataBuffer, url.parameters);
            }
        }
        strcat(http->requestDataBuffer, " ");
        strcat(http->requestDataBuffer, HTTP_VERSION_HEADER);
        strcat(http->requestDataBuffer, NEW_LINE);
        addHeadersToRequest(http->requestDataBuffer);

        if (method == HTTP_POST) {
            addBodyEncodedParamsToRequest(http->requestDataBuffer);
        }

        if (http->sendRequestCallback != NULL) {
            return http->sendRequestCallback(&url, http->requestDataBuffer, requestDataLength, isBlockingExecute);
        }
    }
    return httpResponse;
}

static uint32_t getUrlEncodedParamsLength() {
    uint32_t paramLength = 0;
    uint32_t parametersCount = getHashMapSize(queryParameterMap);
    if (parametersCount > 0) {// count of all "?", "="(2x) and "&" symbols. Total: 4 chars per each parameter if more than 1
        paramLength++; //  add start "?"
        paramLength += parametersCount; // each parameter "="
        paramLength += parametersCount - 1; // and "&" when parameters more than 1
        paramLength += countParameterLength();
    }
    return paramLength;
}

static uint32_t getBodyEncodedParamsLength() {
    uint32_t paramLength = 0;
    uint32_t parametersCount = getHashMapSize(queryParameterMap);
    if (parametersCount > 0) {
        paramLength += parametersCount; // each parameter "="
        paramLength += parametersCount - 1; // and "&" when parameters more than 1
        paramLength += countParameterLength();
    }
    return paramLength;
}

static uint32_t countParameterLength() {
    uint32_t paramLength = 0;
    HashMapIterator iterator = getHashMapIterator(queryParameterMap);
    while (hashMapHasNext(&iterator)) {
        paramLength += strlen(iterator.key);
        paramLength += strlen(iterator.value);
    }
    return paramLength;
}

static uint32_t getHeadersLength() {
    uint32_t headerLength = 0;
    HashMapIterator iterator = getHashMapIterator(headerMap);
    while (hashMapHasNext(&iterator)) {
        headerLength += strlen(iterator.key);
        headerLength += strlen(HEADER_SEMICOLON);
        headerLength += strlen(iterator.value);
        headerLength += strlen(NEW_LINE);
    }

    if (isHashMapNotEmpty(headerMap)) { // add new line at the headers end
        headerLength += strlen(NEW_LINE);
    }
    return headerLength;
}

static void addUrlEncodedParamsToRequest(char *requestBuffer) {
    HashMapIterator iterator = getHashMapIterator(queryParameterMap);
    uint32_t index = 0;
    while (hashMapHasNext(&iterator)) {
        if (index == 0) {
            strcat(requestBuffer, "?");
        } else {
            strcat(requestBuffer, "&");
        }
        strcat(requestBuffer, iterator.key);
        strcat(requestBuffer, "=");
        strcat(requestBuffer, iterator.value);
        index++;
    }
}

static void addHeadersToRequest(char *requestBuffer) {
    HashMapIterator iterator = getHashMapIterator(headerMap);
    while (hashMapHasNext(&iterator)) {
        strcat(requestBuffer, iterator.key);
        strcat(requestBuffer, HEADER_SEMICOLON);
        strcat(requestBuffer, iterator.value);
        strcat(requestBuffer, NEW_LINE);
    }

    if (isHashMapNotEmpty(headerMap)) { // add new line at the headers end
        strcat(requestBuffer, NEW_LINE);
    }
}

static void addBodyEncodedParamsToRequest(char *requestBuffer) {
    HashMapIterator iterator = getHashMapIterator(queryParameterMap);
    uint32_t index = 0;
    while (hashMapHasNext(&iterator)) {
        if (isStringNotEmpty(iterator.key)) {   // for empty string, means json
            if (index > 0) {
                strcat(requestBuffer, "&");
            }
            strcat(requestBuffer, iterator.key);
            strcat(requestBuffer, "=");
        }
        strcat(requestBuffer, iterator.value);
        index++;
    }

    if (isHashMapNotEmpty(queryParameterMap)) {
        strcat(requestBuffer, NEW_LINE);
        strcat(requestBuffer, NEW_LINE);
    }
}
