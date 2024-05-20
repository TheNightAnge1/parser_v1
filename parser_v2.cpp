/*
    1. создать vector <string>
    2. построчно считать весь файл
    3. Разметить блоки данных - определить, какие строки к какому диапазону относятся. Проверка на корректность разметки (все ли блоки есть)
    4. Удалить комментарии, символы переноса, символы начала и окончания блоков данных (по хорошему, можно поудалять все символы, кроме цифр, ".", " ")
    5. Проверка на корректность блока GRID
    6. Создание и заполнение структуры header
    7. Проверка на корректность блоков TEMP и TUBE
    9. Создание структуры data
    8. Считывание данных из TEMP и TUBE, заполнение структуры data

    Проверки:
    1. Наличие и количество ключевых слов (GRID, TUBE, TEMP) 1
    2. Наличие и количество знаков окончания секции 1

    3. Максимальное количество разбиений балки, максимальное количество времён замеров 1
    4. Корректное значение длины балки 1

    5. Начальное время = 0 1
    6. Нет отрицательных моментов времени 1
    7. Отрезки времени идут по порядку 1
    8. Все начальные температуры присутствуют (в первой секции нет температур со значением -1) 1
    9. Всегда заданы температуры на концах балки (в блоках температуры нет значений -1 в начале и в конце блока) 1
    10. Нет отрицательных теплопроводностей 1
    

    11. Размеры секций TEMP и TUBE соответствуют размерам, заданным в секции GRID 1
    12. Допилить чтение файла - удаление строк, полностью состоящих из комментариев. Финальная проверка массива на соответствие. 1

    13. Проверки секций на то, что они состоят только из чисел при записи

    Заметки:
    1. verificator должен проводить проверки не в data, а в strings, перед записью в data
*/

#include "parser_v1.hpp"
using namespace std;
namespace fs = std::filesystem;

#define file_name "input_files\\BULK_1.txt"

struct header_struct
{
    double length = 0;
    uint32_t space_segments = 0;
    uint32_t time_segments = 0;
};

struct data_struct
{
    header_struct header;
    double* koefs;
    double* temps;
    double* time_scale;

    data_struct(header_struct in_header)
    {
        this->header.length = in_header.length;
        this->header.space_segments = in_header.space_segments;
        this->header.time_segments = in_header.time_segments;
        this->koefs = new double[this->header.time_segments];
        this->temps = new double[(this->header.time_segments) * (this->header.space_segments)];
        this->time_scale = new double[this->header.time_segments];
    }

    ~data_struct()
    {
        delete[] koefs;
        delete[] temps;
        delete[] time_scale;
    };
};

//построчное считывание файла
bool file_reader(vector<string>& vec)
{
    bool error_flag = 0;
    string line = "";

    ifstream input_file;
    input_file.open(file_name);
    if (!input_file.is_open())
    {
        cout << "\nWarning! File could not be open\n" << endl;
        error_flag = 1;
    }
    else
    {
        while (getline(input_file, line))
        {
            vec.push_back(line);
        }
        cout << "File readed successfuly" << endl;
    }
    input_file.close();

    return error_flag;
}

//разметка считанных строк
bool parser(vector<string>& vec)
{
    int grid_count = 0;
    int tube_count = 0;
    int temp_count = 0;
    int slash_count = 0;
    bool error_flag = 1;

    //удаление лишних символов из строк
    for (int i = 0; i < vec.size(); i++)
    {
        //удаление символов конца блока "/" и всего, что за ними
        if (vec[i].find("/") != string::npos)
        {
            vec[i].erase(vec[i].begin() + vec[i].find("/"), vec[i].end());
        }

        //удаление символов комментария "--" и всего, что за ними
        if (vec[i].find("--") != string::npos)
        {
            vec[i].erase(vec[i].begin() + vec[i].find("--"), vec[i].end());
        }

        //удаление пробелов в начале
        if (vec[i][0] == ' ')
        {
            while (vec[i][0] == ' ')
            {
                vec[i].erase(vec[i].begin());
            }
        }

        //удаление пробелов в конце
        if (vec[i] != "")
        {
            if (vec[i][vec[i].size() - 1] == ' ')
            {
                while (vec[i][vec[i].size() - 1] == ' ')
                {
                    vec[i].pop_back();
                }
            }
        } 

        //добавление символа окончания строки. Приведение всех строк к виду "data[0] data[1] ... data[n]/"
        if (vec[i] != "")
        {
            vec[i].push_back('/');
        }
    }

    //удаление пустых строк
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == "")
        {
            vec.erase(vec.begin() + i);
        }
    }

    // проверка на то, присутствуют ли все слова начала блоков данных и символы окончания
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j{}; j <= vec[i].length() - 4; )
        {
            int position = vec[i].find("GRID/", j);
            if (position == string::npos) break;
            grid_count++;
            j = position + 1;
        }
        for (int j{}; j <= vec[i].length() - 4; )
        {
            int position = vec[i].find("TUBE/", j);
            if (position == string::npos) break;
            tube_count++;
            j = position + 1;
        }
        for (int j{}; j <= vec[i].length() - 4; )
        {
            int position = vec[i].find("TEMP/", j);
            if (position == string::npos) break;
            temp_count++;
            j = position + 1;
        }
        for (int j{}; j <= vec[i].length() - 1; )
        {
            int position = vec[i].find("/", j);
            if (position == string::npos) break;
            slash_count++;
            j = position + 1;
        }
    }

    if (grid_count == 0)
    {
        cout << "\nWarning! GRID header was not found\n" << endl;
    }
    if (tube_count == 0)
    {
        cout << "\nWarning! TUBE header was not found\n" << endl;
    }
    if (temp_count == 0)
    {
        cout << "\nWarning! TEMP header was not found\n" << endl;
    }

    if (grid_count > 1)
    {
        cout << "\nWarning! More then one GRID header was found\n" << endl;
    }
    if (tube_count > 1)
    {
        cout << "\nWarning! More then one TUBE header was found\n" << endl;
    }
    if (temp_count > 1)
    {
        cout << "\nWarning! More then one TEMP header was found\n" << endl;
    }
    if (slash_count > 6)
    {
        cout << "\nWarning! Too much / markers\n" << endl;
    }
    if (slash_count < 6)
    {
        cout << "\nWarning! Not enough / markers\n" << endl;
    }

    if (grid_count == 1 && tube_count == 1 && temp_count == 1 && slash_count == 6)
    {
        cout << "Headers test 1 passed" << endl;
        error_flag = 0;
    }

    if (error_flag == 1)
    {
        cout << "\nWarning! Headers test 1 failed\n" << endl;
        return error_flag;
    }

    vector<string> temp_vec;
    temp_vec = vec;
    vec.clear();
    vec.push_back("");
    vec.push_back("");
    vec.push_back("");

    for (int i = 0; i < temp_vec.size(); i++)
    {
        if (temp_vec[i] == "GRID/")
        {
            vec[0] = temp_vec[i + 1];
        }
        if (temp_vec[i] == "TUBE/")
        {
            vec[1] = temp_vec[i + 1];
        }
        if (temp_vec[i] == "TEMP/")
        {
            vec[2] = temp_vec[i + 1];
        }
    }

    return error_flag;
}

//считывает данные для структуры header
bool header_reader(vector<string>& vec, header_struct& header)
{
    bool error_flag = 0;

    ////удаляет лишние символы из строк
    //for (int i = 0; i < vec.size(); i++)
    //{
    //    //удаление символов конца блока "/" и всего, что за ними
    //    if (vec[i].find("/") != string::npos)
    //    {
    //        vec[i].erase(vec[i].begin() + vec[i].find("/"), vec[i].end());
    //    }

    //    //удаление пробелов в начале
    //    if (vec[i][0] == ' ')
    //    {
    //        while (vec[i][0] == ' ')
    //        {
    //            vec[i].erase(vec[i].begin());
    //        }
    //    }

    //    //удаление пробелов в конце
    //    if (vec[i][vec[i].size()-1] == ' ')
    //    {
    //        while (vec[i][vec[i].size() - 1] == ' ')
    //        {
    //            vec[i].pop_back();
    //        }
    //    }
    //    
    //    //добавление символа окончания строки. Приведение всех строк к виду "data[0] data[1] ... data[n]/"
    //    vec[i].push_back('/');
    //}

    //заполняет header
    int i = 0;
    int j = 0;
    string temp_line = "";
    while (i < vec[0].size())
    {
        if (vec[0][i] != ' ' && vec[0][i] != '/')
        {
            temp_line += vec[0][i];
        }
        else
        {
            switch (j)
            {
            case 0:
                header.length = stod(temp_line);
                break;
            case 1:
                header.space_segments = stoi(temp_line);
                break;
            case 2:
                header.time_segments = stoi(temp_line);
                break;
            default:
                cout << "\nWarning! More then 3 arguments in GRID section\n" << endl;
                error_flag = 1;
                return error_flag;;
            }
            j++;
            temp_line = "";
        }
        i++;
    }
    cout << "Headers infilled successfully" << endl;

    return error_flag;
}

//заполняет структуру data данными из секций TEMP и TUBE
bool data_reader(vector<string>& vec, data_struct& data)
{
    bool error_flag = 0;

    //проверка, нужное ли число значений в секции TUBE
    int i = 0;
    int j = 0;
    string temp_line = "";
    while (i < vec[1].size())
    {
        if (vec[1][i] == ' ' || vec[1][i] == '/')
        {
            j++;
        }
        i++;
    }
    if (j != data.header.space_segments)
    {
        cout << "\nWarning! Wrong nuber of conductivity coefficients\n" << endl;
        error_flag = 1;
        return error_flag;
    }

    //заполнение данными из секции TUBE
    i = 0;
    j = 0;
    temp_line = "";
    while (i < vec[1].size())
    {
        if (vec[1][i] != ' ' && vec[1][i] != '/')
        {
            temp_line += vec[1][i];
        }
        else
        {
            data.koefs[j] = stod(temp_line);
            temp_line = "";
            j++;
        }
        i++;
    }
    if (j == data.header.space_segments)
    {
        cout << "Coefficients infilled successfully" << endl;
    }

    //проверка, нужное ли число данных в секции TEMP
    i = 0;
    j = 0;
    int k = data.header.space_segments;
    temp_line = "";

    while (i < vec[2].size())
    {
        if (vec[2][i] == ' ' || vec[2][i] == '/')
        {
            if (k == data.header.space_segments)
            {
                k = 0;
            }
            else
            {
                j++;
                k++;
            }
        }
        i++;
    }
    if (k != data.header.space_segments || j != (data.header.space_segments) * (data.header.time_segments))
    {
        cout << "\nWarning! Wrong nuber of temperatures\n" << endl;
        error_flag = 1;
        return error_flag;
    }

    //заполнение данными из секции TEMP
    i = 0;
    j = 0;
    k = data.header.space_segments;
    temp_line = "";

    while (i < vec[2].size())
    {
        if (vec[2][i] != ' ' && vec[2][i] != '/')
        {
            temp_line += vec[2][i];
        }
        else
        {
            if (k == data.header.space_segments)
            {
                //заполнение разбиений по времени
                data.time_scale[j / data.header.space_segments] = stod(temp_line);
                k = 0;
                temp_line = "";
            }
            else
            {
                //заполнение температур
                data.temps[j] = stod(temp_line);
                temp_line = "";
                j++;
                k++;
            }  
        }
        i++;
    }
    if (k == data.header.space_segments && j == (data.header.space_segments) * (data.header.time_segments))
    {
        cout << "Temperatures infilled successfully" << endl;
    }

    return error_flag;
}

//выполняет проверки на корректность в считанном массиве данных
bool verificator(data_struct& data)
{
    //флаг ошибки: 0 - ошибок нет, 1 - ошибки найдены
    bool error_flag = 0;

    //проверка допустимого значения длины, максимального числа разбиений по длине и по времени
    if (data.header.length <= 0)
    {
        cout << "\nWarning! Incorrect value of bulk's lenth\n" << endl;
        error_flag = 1;
    }
    if (data.header.space_segments <= 0 && data.header.space_segments >= 10000)
    {
        cout << "\nWarning! Incorrect value of space segments\n" << endl;
        error_flag = 1;
    }
    if (data.header.time_segments <= 0 && data.header.time_segments >= 1000)
    {
        cout << "\nWarning! Incorrect value of time segments\n" << endl;
        error_flag = 1;
    }

    //проверка правильности разбиения по времени
    for (int i = 0; i < data.header.time_segments; i++)
    {
        if (data.time_scale[i] != i)
        {
            cout << "\nWarning! Wrong time segments\n" << endl;
            error_flag = 1;
        }
    }

    //проверка правильности секции температур
    //проверка начальных температур
    for (int i = 0; i < data.header.space_segments; i++)
    {
        if (data.temps[i] == -1)
        {
            cout << "\nWarning! Initial temperatures missed\n" << endl;
            error_flag = 1;
        }
    }
    //проверка крайних температур
    for (int i = data.header.space_segments - 1; i < (data.header.space_segments)*(data.header.time_segments) - data.header.space_segments; i += data.header.space_segments)
    {
        if (data.temps[i] == -1 || data.temps[i+1] == -1)
        {
            cout << "\nWarning! Edge temperatures missed\n" << endl;
            error_flag = 1;
        }
    }
    if (data.temps[0] == -1 || data.temps[(data.header.space_segments) * (data.header.time_segments) - 1] == -1)
    {
        cout << "\nWarning! Edge temperatures missed\n" << endl;
        error_flag = 1;
    }

    //проверка правильности секции теплопроводностей
    for (int i = 0; i < data.header.space_segments; i++)
    {
        if (data.koefs[i] < 0)
        {
            cout << "\nWarning! Negative conductivity koefficient\n" << endl;
            error_flag = 1;
        }
    }

    if (!error_flag)
    {
        cout << "Data test passed" << endl;
    }


    return error_flag;
}

int main()
{
    bool error_flag = 0;
    header_struct header;
    vector<string> strings;

    error_flag = file_reader(strings);

    if (!error_flag)
    {
        error_flag = parser(strings);
    }

    if (!error_flag)
    {
        error_flag = header_reader(strings, header);
    }

    if (!error_flag)
    {
        //инициализация структуры data с данными из структуры header
        data_struct data(header);

        if (!error_flag)
        {
            error_flag = data_reader(strings, data);
        }

        if (!error_flag)
        {
            error_flag = verificator(data);
        }
    }

    if (!error_flag)
    {
        cout << "Parsing process finished succsessfully" << endl;
    }
    else
    {
        cout << "Parsing process aborted with errors" << endl;
    }

    return 0;
}



