/*
    1. ������� vector <string>
    2. ��������� ������� ���� ����
    3. ��������� ����� ������ - ����������, ����� ������ � ������ ��������� ���������. �������� �� ������������ �������� (��� �� ����� ����)
    4. ������� �����������, ������� ��������, ������� ������ � ��������� ������ ������ (�� ��������, ����� ��������� ��� �������, ����� ����, ".", " ")
    5. �������� �� ������������ ����� GRID
    6. �������� � ���������� ��������� header
    7. �������� �� ������������ ������ TEMP � TUBE
    9. �������� ��������� data
    8. ���������� ������ �� TEMP � TUBE, ���������� ��������� data

    ��������:
    1. ������� � ���������� �������� ���� (GRID, TUBE, TEMP) 1
    2. ������� � ���������� ������ ��������� ������ 1

    3. ������������ ���������� ��������� �����, ������������ ���������� ����� ������� 1
    4. ���������� �������� ����� ����� 1

    5. ��������� ����� = 0 1
    6. ��� ������������� �������� ������� 1
    7. ������� ������� ���� �� ������� 1
    8. ��� ��������� ����������� ������������ (� ������ ������ ��� ���������� �� ��������� -1) 1
    9. ������ ������ ����������� �� ������ ����� (� ������ ����������� ��� �������� -1 � ������ � � ����� �����) 1
    10. ��� ������������� ����������������� 1
    

    11. ������� ������ TEMP � TUBE ������������� ��������, �������� � ������ GRID 1
    12. �������� ������ ����� - �������� �����, ��������� ��������� �� ������������. ��������� �������� ������� �� ������������. 1

    13. �������� ������ �� ��, ��� ��� ������� ������ �� ����� ��� ������

    �������:
    1. verificator ������ ��������� �������� �� � data, � � strings, ����� ������� � data
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

//���������� ���������� �����
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

//�������� ��������� �����
bool parser(vector<string>& vec)
{
    int grid_count = 0;
    int tube_count = 0;
    int temp_count = 0;
    int slash_count = 0;
    bool error_flag = 1;

    //�������� ������ �������� �� �����
    for (int i = 0; i < vec.size(); i++)
    {
        //�������� �������� ����� ����� "/" � �����, ��� �� ����
        if (vec[i].find("/") != string::npos)
        {
            vec[i].erase(vec[i].begin() + vec[i].find("/"), vec[i].end());
        }

        //�������� �������� ����������� "--" � �����, ��� �� ����
        if (vec[i].find("--") != string::npos)
        {
            vec[i].erase(vec[i].begin() + vec[i].find("--"), vec[i].end());
        }

        //�������� �������� � ������
        if (vec[i][0] == ' ')
        {
            while (vec[i][0] == ' ')
            {
                vec[i].erase(vec[i].begin());
            }
        }

        //�������� �������� � �����
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

        //���������� ������� ��������� ������. ���������� ���� ����� � ���� "data[0] data[1] ... data[n]/"
        if (vec[i] != "")
        {
            vec[i].push_back('/');
        }
    }

    //�������� ������ �����
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == "")
        {
            vec.erase(vec.begin() + i);
        }
    }

    // �������� �� ��, ������������ �� ��� ����� ������ ������ ������ � ������� ���������
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

//��������� ������ ��� ��������� header
bool header_reader(vector<string>& vec, header_struct& header)
{
    bool error_flag = 0;

    ////������� ������ ������� �� �����
    //for (int i = 0; i < vec.size(); i++)
    //{
    //    //�������� �������� ����� ����� "/" � �����, ��� �� ����
    //    if (vec[i].find("/") != string::npos)
    //    {
    //        vec[i].erase(vec[i].begin() + vec[i].find("/"), vec[i].end());
    //    }

    //    //�������� �������� � ������
    //    if (vec[i][0] == ' ')
    //    {
    //        while (vec[i][0] == ' ')
    //        {
    //            vec[i].erase(vec[i].begin());
    //        }
    //    }

    //    //�������� �������� � �����
    //    if (vec[i][vec[i].size()-1] == ' ')
    //    {
    //        while (vec[i][vec[i].size() - 1] == ' ')
    //        {
    //            vec[i].pop_back();
    //        }
    //    }
    //    
    //    //���������� ������� ��������� ������. ���������� ���� ����� � ���� "data[0] data[1] ... data[n]/"
    //    vec[i].push_back('/');
    //}

    //��������� header
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

//��������� ��������� data ������� �� ������ TEMP � TUBE
bool data_reader(vector<string>& vec, data_struct& data)
{
    bool error_flag = 0;

    //��������, ������ �� ����� �������� � ������ TUBE
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

    //���������� ������� �� ������ TUBE
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

    //��������, ������ �� ����� ������ � ������ TEMP
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

    //���������� ������� �� ������ TEMP
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
                //���������� ��������� �� �������
                data.time_scale[j / data.header.space_segments] = stod(temp_line);
                k = 0;
                temp_line = "";
            }
            else
            {
                //���������� ����������
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

//��������� �������� �� ������������ � ��������� ������� ������
bool verificator(data_struct& data)
{
    //���� ������: 0 - ������ ���, 1 - ������ �������
    bool error_flag = 0;

    //�������� ����������� �������� �����, ������������� ����� ��������� �� ����� � �� �������
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

    //�������� ������������ ��������� �� �������
    for (int i = 0; i < data.header.time_segments; i++)
    {
        if (data.time_scale[i] != i)
        {
            cout << "\nWarning! Wrong time segments\n" << endl;
            error_flag = 1;
        }
    }

    //�������� ������������ ������ ����������
    //�������� ��������� ����������
    for (int i = 0; i < data.header.space_segments; i++)
    {
        if (data.temps[i] == -1)
        {
            cout << "\nWarning! Initial temperatures missed\n" << endl;
            error_flag = 1;
        }
    }
    //�������� ������� ����������
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

    //�������� ������������ ������ �����������������
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
        //������������� ��������� data � ������� �� ��������� header
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



