#ifndef CODE_FIXED_H
#define CODE_FIXED_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERNAME_LEN 39
#define SETTINGS_COUNT 10
#define MAX_USERS 100
#define INVALID_USER_ID -1

int userid_next = 0;

typedef struct {
    char username[MAX_USERNAME_LEN + 1]; // 널 포함 최대 40바이트
    bool isAdmin;
    long userid;
    long setting[SETTINGS_COUNT];
} user_account;

user_account *accounts[MAX_USERS];

// 사용자 계정 생성 함수
int create_user_account(bool isAdmin, const char *username) {
    if (userid_next >= MAX_USERS) {
        fprintf(stderr, "Error: maximum number of users exceeded.\n");
        return INVALID_USER_ID;
    }

    if (strlen(username) > MAX_USERNAME_LEN) {
        fprintf(stderr, "Error: username too long.\n");
        return INVALID_USER_ID;
    }

    user_account *ua = malloc(sizeof(user_account));
    if (ua == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        return INVALID_USER_ID;
    }

    ua->isAdmin = isAdmin;
    ua->userid = userid_next;

    strncpy(ua->username, username, MAX_USERNAME_LEN);
    ua->username[MAX_USERNAME_LEN] = '\0';

    memset(&ua->setting, 0, sizeof ua->setting);

    accounts[ua->userid] = ua;
    return userid_next++;
}

// 수정된 username 교체 함수 (취약점 제거)
bool update_username(int user_id, const char *new_name) {
    if (user_id < 0 || user_id >= MAX_USERS || accounts[user_id] == NULL)
        return false;

    size_t new_name_len = strlen(new_name);
    if (new_name_len > MAX_USERNAME_LEN) {
        fprintf(stderr, "Error: new username is too long.\n");
        return false;
    }

    strncpy(accounts[user_id]->username, new_name, MAX_USERNAME_LEN);
    accounts[user_id]->username[MAX_USERNAME_LEN] = '\0';

    return true;
}

// 사용자 설정 변경 함수
bool update_setting(int user_id, const char *index, const char *value) {
    if (user_id < 0 || user_id >= MAX_USERS || accounts[user_id] == NULL)
        return false;

    char *endptr;
    long i = strtol(index, &endptr, 10);
    if (*endptr || i < 0 || i >= SETTINGS_COUNT)
        return false;

    long v = strtol(value, &endptr, 10);
    if (*endptr)
        return false;

    accounts[user_id]->setting[i] = v;
    return true;
}

// 관리자 여부 확인 함수
bool is_admin(int user_id) {
    if (user_id < 0 || user_id >= MAX_USERS || accounts[user_id] == NULL) {
        fprintf(stderr, "Error: invalid user id.\n");
        return false;
    }
    return accounts[user_id]->isAdmin;
}

// 사용자 이름 반환 함수
const char* username(int user_id) {
    if (user_id < 0 || user_id >= MAX_USERS || accounts[user_id] == NULL) {
        fprintf(stderr, "Error: invalid user id.\n");
        return NULL;
    }
    return accounts[user_id]->username;
}

#endif // CODE_FIXED_H
