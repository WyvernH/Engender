#include <stdio.h>
#include <stdbool.h>

bool get_first_composition(int n, int k, int composition[k])
{
    if (n < k) {
        return false;
    }
    for (int i = 0; i < k - 1; i++) {
        composition[i] = 1;
    }
    composition[k - 1] = n - k + 1;
    return true;
}

bool get_next_composition(int n, int k, int composition[k])
{
    if (composition[0] == n - k + 1)    {
        return false;
    }
    // there'a an i with composition[i] > 1, and it is not 0.
    // find the last one
    int last = k - 1;
    while (composition[last] == 1) {
        last--;
    }
    // turn    a b ...   y   z 1 1 ...   1
    //                       ^ last
    // into    a b ... (y+1) 1 1 1 ... (z-1)

    // be careful, there may be no 1's at the end

    int z = composition[last];
    composition[last - 1] += 1;
    composition[last] = 1;
    composition[k - 1] = z - 1;
    return true;
}

void display_composition(int k, int composition[k])
{
    char *separator = "[";
    for (int i = 0; i < k; i++) {
        printf("%s%d", separator, composition[i]);
        separator = ",";
    }
    printf("]\n");
}


void display_all_compositions(int n, int k)
{
    int composition[k];  // VLA. Please don't use silly values for k
    for (bool exists = get_first_composition(n, k, composition);
            exists;
            exists = get_next_composition(n, k, composition)) {
        display_composition(k, composition);
    }
}

int main()
{
    display_all_compositions(5, 3);
}
