// Native code programmers should include this file.
#pragma once
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <string>
#include <vector>

extern "C" {
#endif

enum lni_type {
    LNI_NIL,
    LNI_INT,
    LNI_STRING,
    LNI_LIST,
    LNI_LAMBDA
};

typedef int lni_lambda;

typedef struct lni_object lni_object;
struct lni_object{
    union {
        int as_int;
        char* as_string;
        struct {
            lni_object** as_list;
            size_t as_list_len;
        };
        const lni_lambda as_lambda_id;
    };
    enum lni_type type;
};

lni_object* lni_new_nil() {
    lni_object* ret = (lni_object*) malloc(sizeof(lni_object));
    ret->type = LNI_NIL;
    return ret;
}

lni_object* lni_new_int(int val) {
    lni_object* ret = (lni_object*) malloc(sizeof(lni_object));
    ret->type = LNI_INT;
    ret->as_int = val;
    return ret;
}

lni_object* lni_new_string(char* val) {
    lni_object* ret = (lni_object*) malloc(sizeof(lni_object));
    ret->type = LNI_STRING;
    ret->as_string = val;
    return ret;
}

#ifdef __cplusplus
lni_object* lni_new_string_cpp(std::string val) {
    char str[val.length() + 1];
    memcpy(str, val.data(), val.length() + 1);
    return lni_new_string(str);
}
#endif

lni_object* lni_new_list(lni_object** val, size_t len) {
    lni_object* ret = (lni_object*) malloc(sizeof(lni_object));
    ret->type = LNI_LIST;
    ret->as_list = val;
    ret->as_list_len = len;
    return ret;
}

#ifdef __cplusplus
lni_object* lni_new_list_vector(const std::vector<lni_object*>& vec) {
    lni_object** dat = (lni_object**) new lni_object*[vec.size()];
    memcpy(dat, vec.data(), vec.size());
    return lni_new_list(dat, vec.size());
}

// template <typename Iterator>
// lni_object* lni_new_list_cpp(Iterator begin, Iterator end) {
//     std::vector<lni_object> vec;
//     for (auto it = begin; it != end; ++it) vec.push_back(*it);
//     return lni_new_list_vector(vec);
// }

#endif

#ifdef __cplusplus
}
#endif