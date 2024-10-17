#include <iostream>
#include "quicksort.h"
//int step = 0; 
int quickSort(int* array, int beg, int end)
{
    //std::cout << "step = " << step << std::endl;
    //step++;
    //
    //for (int i = beg; i <= end; i++)
    //{
    //    std::cout << array[i] << " ";
    //}
    //std::cout << std::endl;
    int pivot = array[end];
    int i = beg - 1;
    int temp;
    for (int j = beg; j < end; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    i++;
    temp = array[i];
    array[i] = pivot;
    array[end] = temp;

    //for (int i = beg; i <= end; i++)
    //{
    //    std::cout << array[i] << " ";
    //}
    //std::cout << std::endl;

    for (int j = beg; j < i - 1; j++)
    {
        if (array[j] > array[j + 1]) 
        {  
            quickSort(array, beg, i - 1);
            break; 
        }
    }
    for (int j = i + 1; j < end; j++)
    {
        if (array[j] > array[j + 1]) 
        {  
            quickSort(array, i + 1, end);
            break; 
        }
    }
    return 0;
}

//int main ()
//{
//    int a[100];
//    for (int i = 0; i < 99; i++)
//    {
//        a[i] = i + 1;
//    }
//    a[99] = 1;
//    quickSort(&a[0], 0, 99);
//    for (int i = 0; i <= 99; i++)
//    {
//        std::cout << a[i] << " ";
//    }
//    std::cout << std::endl;
//    return 0;
//}