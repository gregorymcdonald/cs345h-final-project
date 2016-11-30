// Native code programmers should include this file.

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
        lni_lambda as_lambda_id;
    };
    enum lni_type type;
};

lni_object* clone_lni_object(lni_object* l){
    lni_object* clone = (lni_object*) malloc(sizeof(lni_object));
    clone->type = l->type;
    switch(l->type){
        case LNI_INT:
        {   
            clone->as_int = l->as_int;
            break;
        }
        case LNI_STRING:
        {
            size_t string_length = strlen(l->as_string) + 1;
            clone->as_string = (char *)malloc(string_length);
            memcpy(clone->as_string, l->as_string, string_length);
            break;
        }
        case LNI_LIST:
        {
            clone->as_list = (lni_object**)malloc(sizeof(lni_object*) * l->as_list_len);
            lni_object** original_iterator = l->as_list;
            lni_object** clone_iterator = clone->as_list;
            for(size_t i = 0; i < l->as_list_len; ++i){
                *clone_iterator = clone_lni_object(*original_iterator);

                ++clone_iterator;
                ++original_iterator;
            }

            clone->as_list_len = l->as_list_len;
            break;
        }
        case LNI_LAMBDA:
        {
            clone->as_lambda_id = l->as_lambda_id;
            break;
        }
        default: 
        {
            clone = NULL;
            break;
        }
    }
    return clone;
}

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

lni_object* lni_new_string(char* val, size_t len) {
    lni_object* ret = (lni_object*) malloc(sizeof(lni_object));
    ret->type = LNI_STRING;
    ret->as_string = (char *)malloc(len + 1);
    memcpy(ret->as_string, val, len + 1);
    return ret;
}

#ifdef __cplusplus
lni_object* lni_new_string_cpp(std::string val) {
    char* str = (char *)malloc(val.length() + 1);
    memcpy(str, val.data(), val.length() + 1);
    return lni_new_string(str, val.length());
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
    // Allocate array of lni_object pointers
    lni_object** dat = (lni_object**) malloc(sizeof(lni_object*) * vec.size());
    lni_object** it = dat;
    for(unsigned int i = 0; i < vec.size(); i++){
        *it = clone_lni_object(vec.at(i));
        ++it;
    }
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