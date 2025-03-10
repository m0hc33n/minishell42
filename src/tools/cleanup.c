#include "../../inc/tools.h"

void    minishell_free_arr(char **arr)
{
    uint64_t    i;

    i = 0;
    if (arr)
    {
        while (arr[i])
        {
            free(arr[i]);
			arr[i] = NULL;
            i++;
        }
        free(arr);
		arr = NULL;
    }
}
