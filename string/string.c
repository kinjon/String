#include <stdio.h>
/*
 * 过滤掉字符串中的版本信息
 * 版本信息:-vxxxx, 其中xxxx全是数字,v12abc不算是版本的信息
 * e.g. sf16a18-p10-v2-gmac ==> sf16a18-p10-gmac
 * */
int grep_version(char * model, char * board)
{
	char str[64] = {0};
	char *ptr1 = NULL;
	char *ptr2 = NULL;
	int flag = 0;
	int len = 0;
	ptr1 = str;

	if (NULL == model || NULL == board)
		return -1;

	printf("model is %s\n", model);
	while (*model != '\0') {
		if (*model == '-' && (*(model + 1) == 'v' || *(model + 1) == 'V')) {
			ptr2 = model;
			model += 2;
			while (*model != '\0' && *model != '-') {
				flag = 1;
				if (*model < '0' || *model > '9') {
					flag = 0;
					break;
				}
				model++;
			}
			if (!flag)
				model = ptr2;
			if (*model == '\0') {
				ptr1++;
				break;
			}
		}
		*ptr1 = *model;
		printf("%c\n", *ptr1);
		model++;
		len++;
		ptr1++;
	}
	if (len > 0) {
		*ptr1 = '\0';
		printf("str is %s len is %d\n", str, len);
		printf("strlen = %d\n", strlen(str));
		memcpy(board, str, strlen(str) + 1);
	}
	return len;
}

#if 0
int sfax8_grep_version(char * model, char * board)
{
	char str[64] = {0};
	char *ptr = NULL;
	int len = 0;
	ptr = str;

	if (NULL == model || NULL == board)
		return -1;

	printf("model is %s\n", model);
	while (*model != '\0') {
		if (*model == '-' && (*(model + 1) == 'v' || *(model + 1) == 'V')) {
			model += 2;
			while (*model != '\0' && *model != '-') {
				model++;
			}
			if (*model == '\0') {
				if (ptr != str)
					ptr++;
				break;
			}
		}
		*(ptr++) = *(model++);
		len++;
	}
	*ptr = '\0';
	printf("str is %s len is %d\n", str, len);
	printf("strlen = %d\n", strlen(str));
	memcpy(board, str, strlen(str) + 1);
	return len;
}
#endif

/*
 * 以-号作为分隔符分割字符串,首先判断第一个字符串,因为第一个字符串不是以-号开头
 * */

int _sfax8_grep_version(const char *str) {
	int len = 0;
	char *ptr = str;
	int flag = 1;
	if (*ptr == 'v' || *ptr == 'V') {
		flag = 0;
		ptr += 1;
		len++;
	}
	while (*ptr != '\0' && *ptr != '-') {
		if (*ptr < '0' || *ptr > '9')
			flag = 1;
		ptr++;
		len++;
	}
	return flag ? len : 0;

}

int sfax8_grep_version(char *model, char *board)
{
	char str[64] = {0};
	char *p_str = board;
	char *p_tmp = model;
	int len = 0;
	int count = 0;
	if (NULL == model || NULL == board)
		return 0;
	len = _sfax8_grep_version(model);
	if (len) {
		memcpy(p_str, model, len);
		p_str += len;
		count += len;
	}

	while (p_tmp && (p_tmp = strstr(p_tmp, "-"))) {
		if (p_tmp) {
			len = _sfax8_grep_version(p_tmp + 1);
			if (len) {
				memcpy(p_str, p_tmp, len + 1);
				p_str += len + 1;
				count += len + 1;
			}
			p_tmp += len + 1;
		}
	}
	*(board + count) = '\0';
	return count;
}

int main()
{
	char *model0 = "-sf16a18-p10m-v2-gmac-hxx-xxpp";
	char *model1 = "-sf16-v9-a18-v7-86v-v2-v4";
	char *model2 = "sf16a18-rep-v1";
	char *model3 = "-sf16a18-v2367-v2-gmac-hxx-xxpp";
	char board0[64] = {0};
	char board1[64] = {0};
	char board2[64] = {0};
	char board3[64] = {0};
	sfax8_grep_version(model0, board0);
	sfax8_grep_version(model1, board1);
	sfax8_grep_version(model2, board2);
	sfax8_grep_version(model3, board3);
	printf("board is %s\n", board0);
	printf("board is %s\n", board1);
	printf("board is %s\n", board2);
	printf("board is %s\n", board3);
	return 0;
}
