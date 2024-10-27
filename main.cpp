#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

template<class T>
class MultiArray {
private:
    int numofarrays;
    T **arrays;
    int *sizes;
    string *names;
    int multiArrSize;
public:
    MultiArray() {
        sizes = nullptr;
        names = nullptr;
        arrays = nullptr;

    }

    MultiArray(int n, int *si, string *na) {
        numofarrays = n;
        sizes = new int[numofarrays];
        names = new string[numofarrays];
        arrays = new T *[numofarrays];
        for (int i = 0; i < numofarrays; i++) {
            sizes[i] = si[i];
            names[i] = na[i];
            arrays[i] = new T[sizes[i]];
        }
    }

    ~MultiArray() {
        for (int i = 0; i < numofarrays; i++) {
            delete[] arrays[i];
        }
        delete[]arrays;
        delete[]sizes;
        delete[]names;
    }

    MultiArray(const MultiArray &obj) {
        numofarrays = obj.numofarrays;
        sizes = new int[numofarrays];
        names = new string[numofarrays];
        arrays = new T *[numofarrays];
        for (int i = 0; i < numofarrays; i++) {
            sizes[i] = obj.sizes[i];
            names[i] = obj.names[i];
            arrays[i] = new T[sizes[i]];
            for (int j = 0; j < sizes[i]; j++) {
                arrays[i][j] = obj.arrays[i][j];
            }
        }
    }

    void loadFromFile(string arrName, string fileName) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == arrName) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw arrName;
            }
        }
        ifstream infile(fileName, ios::in);
        T container;
        infile.clear();
        for (int i = 0; i < sizes[indicator]; i++) {
            infile >> container;
            arrays[indicator][i] = container;
        }
    }

    T *subArray(string name, int startIndex, int endIndex, int step = 1) {
        if (startIndex < endIndex) {
            int indicator;
            for (int i = 0; i < numofarrays; i++) {
                if (names[i] == name) {
                    indicator = i;
                    break;
                }
                if (i == numofarrays - 1) {
                    throw name;
                }
            }
            if (startIndex < 0 || startIndex >= sizes[indicator] || endIndex < 0 || endIndex >= sizes[indicator]) {
                throw 1;
            }
            T *newarr;
            int newarrsize = 0;
            for (int i = startIndex; i <= endIndex; i += step) {
                newarrsize++;
            }
            newarr = new T[newarrsize];
            int j = 0;
            for (int i = startIndex; i <= endIndex; i += step) {
                newarr[j] = arrays[indicator][i];
                j++;
            }
            return newarr;
        } else if (startIndex > endIndex) {
            int indicator;
            for (int i = 0; i < numofarrays; i++) {
                if (names[i] == name) {
                    indicator = i;
                    break;
                }
                if (i == numofarrays - 1) {
                    throw name;
                }
            }
            if (startIndex < 0 || startIndex >= sizes[indicator] || endIndex < 0 || endIndex >= sizes[indicator]) {
                throw 1;
            }
            T *newarr;
            int newarrsize = 0;
            for (int i = endIndex; i <= startIndex; i += step) {
                newarrsize++;
            }
            newarr = new T[newarrsize];
            int j = 0;
            for (int i = startIndex; i >= endIndex; i -= step) {
                newarr[j] = arrays[indicator][i];
                j++;
            }
            return newarr;
        } else {
            throw 3.5;
        }
    }

    void reverse(string name) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == name) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw name;
            }
        }
        int j = sizes[indicator] - 1;
        for (int i = 0; i < (sizes[indicator] / 2); i++) {
            T temp = arrays[indicator][i];
            arrays[indicator][i] = arrays[indicator][j];
            arrays[indicator][j] = temp;
            j--;
        }
    }

    void Sort(string name) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == name) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw name;
            }
        }
        sort(arrays[indicator], (arrays[indicator] + sizes[indicator]));
    }

    bool search(T item) {
        for (int i = 0; i < numofarrays; i++) {
            for (int j = 0; j < sizes[i]; j++) {
                if (arrays[i][j] == item) {
                    return true;
                }
            }
        }
        return false;
    }

    T &operator()(string name, int index) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == name) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw name;
            }
        }
        if (index >= 0 && index < sizes[indicator]) {
            return arrays[indicator][index];
        } else if (index < 0 && index >= (sizes[indicator] * -1)) {
            return (arrays[indicator][sizes[indicator] + index]);
        } else
            throw index;
    }

    MultiArray split(string name) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == name) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw name;
            }
        }
        string *newNames;
        int *newSizes;
        newNames = new string[2];
        newSizes = new int[2];
        newNames[0] = "first";
        newNames[1] = "second";
        if (sizes[indicator] % 2 == 0) {
            newSizes[0] = sizes[indicator] / 2;
            newSizes[1] = sizes[indicator] / 2;
        } else {
            newSizes[0] = sizes[indicator] / 2;
            newSizes[1] = (sizes[indicator] / 2) + 1;
        }
        MultiArray<T> splitedArr(2, newSizes, newNames);
        int key = (sizes[indicator] / 2) - 1;
        splitedArr.arrays[0] = this->subArray(name, 0, key);
        splitedArr.arrays[1] = this->subArray(name, key + 1, sizes[indicator] - 1);
        return splitedArr;
    }

    T *merge() {
        T *result;
        int resSize = 0;
        for (int i = 0; i < numofarrays; i++) {
            resSize += sizes[i];
        }
        result = new T[resSize];
        int indicator = 0;
        for (int i = 0; i < numofarrays; i++) {
            for (int j = 0; j < sizes[i]; j++) {
                result[indicator] = arrays[i][j];
                indicator++;
            }

        }
        return result;
    }

    void applyToArray(string name, void (*func)(T &)) {
        int indicator;
        for (int i = 0; i < numofarrays; i++) {
            if (names[i] == name) {
                indicator = i;
                break;
            }
            if (i == numofarrays - 1) {
                throw name;
            }
        }
        for (int i = 0; i < sizes[indicator]; i++) {
            func(arrays[indicator][i]);

        }
    }

    friend ostream &operator<<(ostream &os, const MultiArray &obj) {
        for (int i = 0; i < obj.numofarrays; i++) {
            int c = obj.sizes[i] - 1;
            cout << "{";
            for (int j = 0; j < obj.sizes[i]; j++) {
                cout << obj.arrays[i][j];
                if (c--) {
                    cout << ",";
                }
            }
            cout << "}";
            cout << endl;
        }
    }

    int getmultiArrSize() {
        multiArrSize = 0;
        for (int i = 0; i < numofarrays; i++) {
            multiArrSize += sizes[i];
        }
        return multiArrSize;
    }
};

template<class T2>
void square(T2 &x) {
    x = x * x;
}

int main() {
    int numofArr;
    cout << "How many arrays do you want? ";
    cin >> numofArr;
    int arraysSizes[numofArr];
    string arraysNames[numofArr];
    for (int i = 0; i < numofArr; i++) {
        cout << "enter the name of array " << i + 1 << " : ";
        cin >> arraysNames[i];
        cout << "enter the size of array " << i + 1 << " : ";
        cin >> arraysSizes[i];
    }
    MultiArray<int> myArr(numofArr, arraysSizes, arraysNames);
    for (int i = 0; i < numofArr; i++) {
        cout << "for array (" << arraysNames[i] << ")" << endl;
        for (int j = 0; j < arraysSizes[i]; j++) {
            cout << "enter element " << j + 1 << " : ";
            cin >> myArr(arraysNames[i], j);
        }
    }
    int choice;
    do {
        cout << "choose an option:-)" << endl
             << "1-Display your Arrays" << endl
             << "===========================================" << endl
             << "2-Load Data from file" << endl
             << "===========================================" << endl
             << "3-copy you Multiarray to another one" << endl
             << "===========================================" << endl
             << "4-Extract a sub array" << endl
             << "===========================================" << endl
             << "5-reverse your an array in your multiarray" << endl
             << "===========================================" << endl
             << "6-sort an array in your multiarray" << endl
             << "===========================================" << endl
             << "7-search for an item in your Multi Array" << endl
             << "===========================================" << endl
             << "8-edit an element in your multiarray" << endl
             << "===========================================" << endl
             << "9-split an array of your multiarray in another maltiarray" << endl
             << "===========================================" << endl
             << "10-merge your multiarray in one array" << endl
             << "===========================================" << endl
             << "11-square an array in your multiarray" << endl
             << "===========================================" << endl
             << "12-exit" << endl
             << "===========================================" << endl;
        cin >> choice;
        switch (choice) {
            case 1 : {
                cout << "your MultiArray" << endl;
                cout << myArr;
                cout << "==================================" << endl;
                break;
            }
            case 2 : {
                string arrName, fileName;
                cout << "please don't forget to make sure that you have created the file  :-)" << endl;
                cout << "enter the name of the array : ";
                cin >> arrName;
                cout << "enter the name of the file : ";
                cin >> fileName;
                try {
                    myArr.loadFromFile(arrName, fileName);
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 3 : {
                MultiArray<int> copiedArr = myArr;
                cout << "the copied array is :" << endl;
                cout << copiedArr;
                cout << "==================================" << endl;
                break;
            }
            case 4 : {
                string arrName;
                int startIndex, endIndex, numOfSteps;
                cout << "enter the name of the array : ";
                cin >> arrName;
                cout << "enter the start Index : ";
                cin >> startIndex;
                cout << "enter the end Index : ";
                cin >> endIndex;
                cout << "enter the number of steps : ";
                cin >> numOfSteps;
                int *subArr;
                try {
                    subArr = myArr.subArray(arrName, startIndex, endIndex, numOfSteps);
                    int sizeofsubArr;
                    if (startIndex < endIndex) {
                        sizeofsubArr = 0;
                        for (int i = startIndex; i <= endIndex; i += numOfSteps) {
                            sizeofsubArr++;
                        }
                    } else {
                        sizeofsubArr = 0;
                        for (int i = endIndex; i <= startIndex; i += numOfSteps) {
                            sizeofsubArr++;
                        }
                    }
                    cout << "the sub array is : " << endl;
                    int c = sizeofsubArr - 1;
                    cout << "{";
                    for (int i = 0; i < sizeofsubArr; i++) {
                        cout << subArr[i];
                        if (c--)
                            cout << ',';
                    }
                    cout << "}" << endl;
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                catch (int num) {
                    cout << "there is an index out of range " << endl;
                }
                catch (double n) {
                    cout << "the start index and the end index can't equal each other " << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 5 : {
                string arrName;
                cout << "enter the name of the array : ";
                cin >> arrName;
                try {
                    myArr.reverse(arrName);
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 6 : {
                string arrName;
                cout << "enter the name of the array : ";
                cin >> arrName;
                try {
                    myArr.Sort(arrName);
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 7 : {
                int item;
                cout << "enter the item that you want to search for : ";
                cin >> item;
                try {
                    if (myArr.search(item)) {
                        cout << "Item is found" << endl;
                    } else
                        cout << "Item is not found" << endl;
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 8 : {
                string arrName;
                int Index;
                cout << "enter the name of the array : ";
                cin >> arrName;
                cout << "enter the index of the element that you want to change : ";
                cin >> Index;
                cout << "Enter the update : ";
                try {
                    cin >> myArr(arrName, Index);
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                catch (int num) {
                    cout << "the Index is out of range " << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 9 : {
                string arrName;
                cout << "enter the name of the array : ";
                cin >> arrName;
                try {
                    MultiArray<int> splitedArr = myArr.split(arrName);
                    cout << "the splited Arr is : " << endl;
                    cout << splitedArr;
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
            case 10 : {
                int *mergedArr = myArr.merge();
                int sizeOfMergedArr = myArr.getmultiArrSize();
                int c = sizeOfMergedArr - 1;
                cout << '{';
                for (int i = 0; i < sizeOfMergedArr; i++) {
                    cout << mergedArr[i];
                    if (c--)
                        cout << ',';
                }
                cout << '}' << endl;
                cout << "==================================" << endl;
                break;
            }
            case 11 : {
                string arrName;
                cout << "enter the name of the array : ";
                cin >> arrName;
                try {
                    myArr.applyToArray(arrName, square);
                }
                catch (string name) {
                    cout << "Array name " << name << " is not found" << endl;
                }
                cout << "==================================" << endl;
                break;
            }
        }
    } while (choice != 12);
    cout << "Exiting the Program";
    return 0;
}