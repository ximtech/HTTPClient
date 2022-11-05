#include "HTTPClient.h"

#define HEADER_MAP_INITIAL_CAPACITY 4
#define QUERY_PARAMETER_MAP_INITIAL_CAPACITY 4
#define HEADER_SEMICOLON ": "
#define NEW_LINE         "\r\n"

static GET get(HTTPClient *client, char *rawUrl);
static GET bindGetParam(HTTPClient *client, const char *key, char *value);
static GET addGetHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static GET addGetCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse executeGet(HTTPClient *client);
static HTTPResponse executeGetNonBlock(HTTPClient *client);

static POST post(HTTPClient *client, char *rawUrl);
static POST bindPostParam(HTTPClient *client, char *key, char *value);
static POST bindJson(HTTPClient *client, const char *jsonString);
static POST addPostHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static POST addPostCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse executePost(HTTPClient *http);
static HTTPResponse executePostNonBlock(HTTPClient *http);

static PUT put(HTTPClient *client, char *rawUrl);
static PUT addPutHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static PUT addPutCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse executePut(HTTPClient *http);
static HTTPResponse executePutNonBlock(HTTPClient *http);

static DELETE aDelete(HTTPClient *client, char *rawUrl);
static DELETE addDeleteHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static DELETE addDeleteCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse executeDelete(HTTPClient *http);
static HTTPResponse executeDeleteNonBlock(HTTPClient *http);

static HEAD head(HTTPClient *client, char *rawUrl);
static HEAD addHeadHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static HEAD addHeadCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse executeHead(HTTPClient *http);
static HTTPResponse executeHeadNonBlock(HTTPClient *http);

static void resetInnerData(HTTPClient *client);
static void bindParam(HTTPClient *client, const char *key, char *value);
static void addHeader(HTTPClient *client, HTTPHeaderKey key, char *value);
static void addCustomHeader(HTTPClient *client, const char *key, char *value);
static HTTPResponse doExecute(HTTPMethod method, HTTPClient *client, bool isBlockingExecute);

static uint32_t getUrlEncodedParamsLength(HTTPClient *client);
static uint32_t getBodyEncodedParamsLength(HTTPClient *client);
static uint32_t countParameterLength(HTTPClient *client);
static uint32_t getHeadersLength(HTTPClient *client);
static void addHeadersToRequest(HTTPClient *client);
static void addUrlEncodedParamsToRequest(HTTPClient *client);
static void addBodyEncodedParamsToRequest(HTTPClient *client);

static GET httpGet = {
        .bindParam = bindGetParam,
        .addHeader = addGetHeader,
        .addCustomHeader = addGetCustomHeader,
        .executeNonBlock = executeGetNonBlock,
        .execute = executeGet,
};
static POST httpPost = {
        .bindParam = bindPostParam,
        .bindJson = bindJson,
        .addHeader = addPostHeader,
        .addCustomHeader = addPostCustomHeader,
        .executeNonBlock = executePostNonBlock,
        .execute = executePost,
};
static PUT httpPut = {
        .addHeader = addPutHeader,
        .addCustomHeader = addPutCustomHeader,
        .executeNonBlock = executePutNonBlock,
        .execute = executePut,
};
static DELETE httpDelete = {
        .addHeader = addDeleteHeader,
        .addCustomHeader = addDeleteCustomHeader,
        .executeNonBlock = executeDeleteNonBlock,
        .execute = executeDelete,
};
static HEAD httpHead = {
        .addHeader = addHeadHeader,
        .addCustomHeader = addHeadCustomHeader,
        .executeNonBlock = executeHeadNonBlock,
        .execute = executeHead,
};


void initHTTPClient(HTTPClient *client, char *dataBuffer, uint32_t size) {
    if (client == NULL) return;
    client->GET = get;
    client->POST = post;
    client->PUT = put;
    client->DELETE = aDelete;
    client->HEAD = head;

    client->requestBuffer = dataBuffer;
    client->requestBufferSize = size;

    initSingletonHashMap(&client->headers, HEADER_MAP_INITIAL_CAPACITY);
    initSingletonHashMap(&client->queryParameters, QUERY_PARAMETER_MAP_INITIAL_CAPACITY);
}

void registerHttpCallback(HTTPClient *client, HTTPResponse (*callbackFunction)(HTTPClient *, uint32_t, bool)) {
    if (client != NULL && callbackFunction != NULL) {
        client->sendRequestCallback = callbackFunction;
    }
}

void deleteHttpClient(HTTPClient *client) {
    if (client != NULL) {
        hashMapDelete(client->headers);
        hashMapDelete(client->queryParameters);
        client->headers = NULL;
        client->queryParameters = NULL;
    }
}

static GET get(HTTPClient *client, char *rawUrl) {
    resetInnerData(client);
    parseUrlString(&client->url, rawUrl);
    return httpGet;
}

static GET bindGetParam(HTTPClient *client, const char *key, char *value) {
    bindParam(client, key, value);
    return httpGet;
}

static GET addGetHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addHeader(client, key, value);
    return httpGet;
}

static GET addGetCustomHeader(HTTPClient *client, const char *key, char *value) {
    addCustomHeader(client, key, value);
    return httpGet;
}

static HTTPResponse executeGet(HTTPClient *client) {
    return doExecute(HTTP_GET, client, true);
}

static HTTPResponse executeGetNonBlock(HTTPClient *client) {
    return doExecute(HTTP_GET, client, false);
}


static POST post(HTTPClient *client, char *rawUrl) {
    resetInnerData(client);
    parseUrlString(&client->url, rawUrl);
    return httpPost;
}

static POST bindPostParam(HTTPClient *client, char *key, char *value) {
    bindParam(client, key, value);
    return httpPost;
}

static POST bindJson(HTTPClient *client, const char *jsonString) {
    hashMapPut(client->queryParameters, "", (char *) jsonString);
    return httpPost;
}

static POST addPostHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addHeader(client, key, value);
    return httpPost;
}

static POST addPostCustomHeader(HTTPClient *client, const char *key, char *value) {
    addCustomHeader(client, key, value);
    return httpPost;
}

static HTTPResponse executePost(HTTPClient *http) {
    return doExecute(HTTP_POST, http, true);
}

static HTTPResponse executePostNonBlock(HTTPClient *http) {
    return doExecute(HTTP_POST, http, false);
}


static PUT put(HTTPClient *client, char *rawUrl) {
    resetInnerData(client);
    parseUrlString(&client->url, rawUrl);
    return httpPut;
}

static PUT addPutHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addHeader(client, key, value);
    return httpPut;
}

static PUT addPutCustomHeader(HTTPClient *client, const char *key, char *value) {
    addCustomHeader(client, key, value);
    return httpPut;
}

static HTTPResponse executePut(HTTPClient *http) {
    return doExecute(HTTP_PUT, http, true);
}

static HTTPResponse executePutNonBlock(HTTPClient *http) {
    return doExecute(HTTP_PUT, http, false);
}


static DELETE aDelete(HTTPClient *client, char *rawUrl) {
    resetInnerData(client);
    parseUrlString(&client->url, rawUrl);
    return httpDelete;
}

static DELETE addDeleteHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addHeader(client, key, value);
    return httpDelete;
}

static DELETE addDeleteCustomHeader(HTTPClient *client, const char *key, char *value) {
    addCustomHeader(client, key, value);
    return httpDelete;
}

static HTTPResponse executeDelete(HTTPClient *http) {
    return doExecute(HTTP_DELETE, http, true);
}

static HTTPResponse executeDeleteNonBlock(HTTPClient *http) {
    return doExecute(HTTP_DELETE, http, false);
}


static HEAD head(HTTPClient *client, char *rawUrl) {
    resetInnerData(client);
    parseUrlString(&client->url, rawUrl);
    return httpHead;
}

static HEAD addHeadHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addHeader(client, key, value);
    return httpHead;
}

static HEAD addHeadCustomHeader(HTTPClient *client, const char *key, char *value) {
    addCustomHeader(client, key, value);
    return httpHead;
}

static HTTPResponse executeHead(HTTPClient *http) {
    return doExecute(HTTP_HEAD, http, true);
}

static HTTPResponse executeHeadNonBlock(HTTPClient *http) {
    return doExecute(HTTP_HEAD, http, false);
}


static void resetInnerData(HTTPClient *client) {
    hashMapClear(client->queryParameters);
    hashMapClear(client->headers);
}

static void bindParam(HTTPClient *client, const char *key, char *value) {
    if (isStringNotBlank(key) && isStringNotBlank(value)) {
        hashMapPut(client->queryParameters, key, value);
    }
}

static void addHeader(HTTPClient *client, HTTPHeaderKey key, char *value) {
    addCustomHeader(client, getHeaderValueByKey(key), value);
}

static void addCustomHeader(HTTPClient *client, const char *key, char *value) {
    if (isStringNotBlank(key) && isStringNotBlank(value)) {
        hashMapPut(client->headers, key, value);
    }
}

static HTTPResponse doExecute(HTTPMethod method, HTTPClient *client, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_NO_STATUS, NULL, NULL};
    if (client != NULL && client->url.isUrlValid && client->requestBuffer != NULL && client->requestBufferSize > 0) {
        uint32_t requestDataLength = 0;
        const char *methodName = getHttpMethodName(method);
        addHeader(client, HOST, client->url.host);  // add required header for HTTPClient/1.1

        requestDataLength += strlen(methodName);
        requestDataLength += strlen(" /");
        requestDataLength += strlen(client->url.path);

        if (method == HTTP_GET) {
            requestDataLength += getUrlEncodedParamsLength(client);
            requestDataLength += isStringNotEmpty(client->url.parameters) ? strlen(client->url.parameters) + 1 : 0; // +1 for delimiter "&" or "?"
        } else if (method == HTTP_POST) {
            uint32_t parametersLength = getBodyEncodedParamsLength(client);
            if (parametersLength > 0) {
                char dataLengthBuffer[sizeof(uint32_t) * 8 + 1];    // u32 full size and +1 for line end
                sprintf(dataLengthBuffer, "%lu", parametersLength);
                addHeader(client, CONTENT_LENGTH, dataLengthBuffer);
            }
            requestDataLength += parametersLength;
        }

        requestDataLength += strlen(" ");
        requestDataLength += strlen(HTTP_VERSION_HEADER);
        requestDataLength += strlen(NEW_LINE);
        requestDataLength += getHeadersLength(client);
        requestDataLength++;     // place for line end

        if (requestDataLength >= client->requestBufferSize) { // check that data fits buffer size
            return httpResponse;
        }
        memset(client->requestBuffer, 0, client->requestBufferSize);
        strcat(client->requestBuffer, methodName);
        strcat(client->requestBuffer, " /");
        strcat(client->requestBuffer, client->url.path);

        if (method == HTTP_GET) {
            addUrlEncodedParamsToRequest(client);
            if (isStringNotEmpty(client->url.parameters)) {
                strcat(client->requestBuffer, isHashMapEmpty(client->queryParameters) ? "?" : "&");
                strcat(client->requestBuffer, client->url.parameters);
            }
        }
        strcat(client->requestBuffer, " ");
        strcat(client->requestBuffer, HTTP_VERSION_HEADER);
        strcat(client->requestBuffer, NEW_LINE);
        addHeadersToRequest(client);

        if (method == HTTP_POST) {
            addBodyEncodedParamsToRequest(client);
        }

        if (client->sendRequestCallback != NULL) {
            return client->sendRequestCallback(client, requestDataLength, isBlockingExecute);
        }
    }
    return httpResponse;
}

static uint32_t getUrlEncodedParamsLength(HTTPClient *client) {
    uint32_t paramLength = 0;
    uint32_t parametersCount = getHashMapSize(client->queryParameters);
    if (parametersCount > 0) {// count of all "?", "="(2x) and "&" symbols. Total: 4 chars per each parameter if more than 1
        paramLength++; //  add start "?"
        paramLength += parametersCount; // each parameter "="
        paramLength += parametersCount - 1; // and "&" when parameters more than 1
        paramLength += countParameterLength(client);
    }
    return paramLength;
}

static uint32_t getBodyEncodedParamsLength(HTTPClient *client) {
    uint32_t paramLength = 0;
    uint32_t parametersCount = getHashMapSize(client->queryParameters);
    if (parametersCount > 0) {
        paramLength += parametersCount; // each parameter "="
        paramLength += parametersCount - 1; // and "&" when parameters more than 1
        paramLength += countParameterLength(client);
    }
    return paramLength;
}

static uint32_t countParameterLength(HTTPClient *client) {
    uint32_t paramLength = 0;
    HashMapIterator iterator = getHashMapIterator(client->queryParameters);
    while (hashMapHasNext(&iterator)) {
        paramLength += strlen(iterator.key);
        paramLength += strlen(iterator.value);
    }
    return paramLength;
}

static uint32_t getHeadersLength(HTTPClient *client) {
    uint32_t headerLength = 0;
    HashMapIterator iterator = getHashMapIterator(client->headers);
    while (hashMapHasNext(&iterator)) {
        headerLength += strlen(iterator.key);
        headerLength += strlen(HEADER_SEMICOLON);
        headerLength += strlen(iterator.value);
        headerLength += strlen(NEW_LINE);
    }

    if (isHashMapNotEmpty(client->headers)) { // add new line at the headers end
        headerLength += strlen(NEW_LINE);
    }
    return headerLength;
}

static void addUrlEncodedParamsToRequest(HTTPClient *client) {
    HashMapIterator iterator = getHashMapIterator(client->queryParameters);
    uint32_t index = 0;
    while (hashMapHasNext(&iterator)) {
        if (index == 0) {
            strcat(client->requestBuffer, "?");
        } else {
            strcat(client->requestBuffer, "&");
        }
        strcat(client->requestBuffer, iterator.key);
        strcat(client->requestBuffer, "=");
        strcat(client->requestBuffer, iterator.value);
        index++;
    }
}

static void addHeadersToRequest(HTTPClient *client) {
    HashMapIterator iterator = getHashMapIterator(client->headers);
    while (hashMapHasNext(&iterator)) {
        strcat(client->requestBuffer, iterator.key);
        strcat(client->requestBuffer, HEADER_SEMICOLON);
        strcat(client->requestBuffer, iterator.value);
        strcat(client->requestBuffer, NEW_LINE);
    }

    if (isHashMapNotEmpty(client->headers)) { // add new line at the headers end
        strcat(client->requestBuffer, NEW_LINE);
    }
}

static void addBodyEncodedParamsToRequest(HTTPClient *client) {
    HashMapIterator iterator = getHashMapIterator(client->queryParameters);
    uint32_t index = 0;
    while (hashMapHasNext(&iterator)) {
        if (isStringNotEmpty(iterator.key)) {   // for empty string, means json
            if (index > 0) {
                strcat(client->requestBuffer, "&");
            }
            strcat(client->requestBuffer, iterator.key);
            strcat(client->requestBuffer, "=");
        }
        strcat(client->requestBuffer, iterator.value);
        index++;
    }

    if (isHashMapNotEmpty(client->queryParameters)) {
        strcat(client->requestBuffer, NEW_LINE);
        strcat(client->requestBuffer, NEW_LINE);
    }
}
