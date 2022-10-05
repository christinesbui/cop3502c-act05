#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Dr. Gerber's code that he provided for us from assignment 1
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';
        t--;
    }
}

// Dr. Gerber's code that he provided for us from assignment 1
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }
    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

// reads each line of the input file
void read_ifp(FILE *ifp, int count, int array[])
{
    char buf[32];
    int num;

    for(int i = 0; i < count; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);
        sscanf(buf, "%d", &num);
        array[i] = num;
    }
}

// swaps values in the array
void swap_values(int index, int swap_index, int array[])
{
    int temp = array[index];
    array[index] = array[swap_index];
    array[swap_index] = temp;
}

// sifts the value down the tree if it is greater than its childrem
void siftdown_values(int parent, int count, int array[])
{
    int left_child = (2 * parent) + 1;
    int right_child = (2 * parent) + 2;

    int k;

    if((left_child >= count || left_child < 0) && (right_child >= count || right_child < 0))
        return;
    else if(left_child >= count || left_child < 0)
        k = right_child;
    else if(right_child >= count || right_child < 0)
        k = left_child;
    else
        k = (array[left_child] >= array[right_child]) ? left_child : right_child;

    // if parent value is smaller than the large child, sift down appears
    if(array[parent] < array[k])
    {
        swap_values(parent, k, array);
        siftdown_values(k, count, array);
    }
}

// prints array
void print_array(FILE *ofp, int count, int array[])
{
    for(int i = 0; i < count; i++)
        fprintf(ofp, "%d", array[i]);
}

// reorganizes array into a heap
void heapify(FILE *ofp, int count, int array[])
{
    for(int i = count; i >= 0; i-=2)
    {
       int parent = trunc((i - 1) / 2);
       if(parent < 0)
            continue;

       siftdown_values(parent, count, array);
       fprintf(ofp, "\n");
       print_array(ofp, count, array);
    }
}

// checks if a value needs to sift down and or decrements and returns the new count
int remove_node(FILE *ofp, int remove, int count, int array[])
{
    fprintf(ofp, "\n%d", array[remove]);

    swap_values(remove, count - 1, array);
    count --;
    siftdown_values(remove, count, array);
    return count;
}

int main()
{
    FILE *ifp;
    FILE *ofp;
    char buf[256];
    int count;

    ifp = fopen("cop3502-as5-input.txt", "r");
    ofp = fopen("cop3502-as5-output-bui-christine.txt", "w");

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d", &count);

    int array[count];
    read_ifp(ifp, count, array);
    print_array(ofp, count, array);
    heapify(ofp, count, array);

    // while loop continues
    while(count != 0)
    {
        count = remove_node(ofp, 0, count, array);
        fprintf(ofp, "\n");
        print_array(ofp, count, array);
    }
    fclose(ifp);
    fclose(ofp);
    return 0;
}
