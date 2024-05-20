//ctrl+k+c
//ctrl+k+u

#include "parser_v1.hpp"

#define file_name "input_files\\BULK_1.txt"

using namespace std;
namespace fs = std::filesystem;

struct header_struct
{
    double length = 0;
    uint32_t space_segments = 0;
    uint32_t time_segments = 0;

};

struct data_struct
{
    header_struct header;
    //double *koefs = new double[header.time_segments * header.space_segments];
    //double* temps = new double[header.time_segments*header.space_segments];
    //double *time_scale = new double[header.time_segments];
    double* koefs;//пока не понимаю, какая должна быть размерность
    double* temps;
    double* time_scale;

    /*double** temps = { new double* [header.time_segments] {} };
    data_struct()
    {
        for (int i = 0; i < header.space_segments; i++)
        {
            temps[i] = new double[header.space_segments] {};
        }
    };*/
    data_struct(double in_length, uint32_t in_space, uint32_t in_time)
    {
        this->header.length = in_length;
        this->header.space_segments = in_space;
        this->header.time_segments = in_time;
        this->koefs = new double[in_space];
        this->temps = new double[in_time * in_space];
        this->time_scale = new double[in_time];
    }

    ~data_struct()
    {
        delete[] koefs;
      /*  for (int i = 0; i < header.space_segments; i++)
        {
            delete[] temps[i];
        }*/
        delete[] temps;
        delete[] time_scale;
    };
};

int old_main()
{
    double length = 0;
    uint32_t space_segments = 0;
    uint32_t time_segments = 0;

    string line = "";
    char sym = ' ';
    
    /*string path = "\input_files";
    for (const auto& entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;*/

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    ifstream input_file;
    input_file.open(file_name);
    if (!input_file.is_open())
    {
        cout << "Warning! File could not be open\n" << endl;;
    }
    else
    {
        /*while (sym != '\0');
        {
            sym = input_file.get();
            line += sym;
            cout << sym << endl;
        };*/
        sym = input_file.get();
        while (sym != '\n')
        {
            line += sym;
            sym = input_file.get();
        };

        cout << line << endl;

        if (line != "GRID")
        {
            cout << "Warning! Incorrect file structure" << endl;
        }
        else
        {
            int i = 0;
            sym = 'X';
            while (sym != '\n')
            {
                line = "";
                sym = input_file.get();
                while (sym != ' ' && sym != '\n' && sym != '/')
                {
                    line += sym;
                    sym = input_file.get();
                };

                switch (i)
                {
                case 0:
                    length = stod(line);
                    break;
                case 1:
                    space_segments = stoi(line);
                    break;
                case 2:
                    time_segments = stoi(line);
                    break;
                default:
                    cout << "Warning! Incorrect GRID section" << endl;
                    break;
                }
                i++;
            };

        }

        data_struct data(length, space_segments, time_segments); // data structure initialising
        cout << data.header.length << " " << data.header.space_segments << " " << data.header.time_segments << endl;

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        int i = 0;
        getline(input_file, line);
        while (line != "TUBE")
        {
            getline(input_file, line);
            i++;
            if (i > 8)
            {
                cout << "Tube_error" << endl;
                input_file.close();
                return 0;
            }
        }
        cout << line << endl;

        i = 0;
        sym = input_file.get();
        while (sym != '/')
        {
            line = "";

            while (sym != ' ' && sym != '\n')
            {
                line += sym;
                sym = input_file.get();
            }
            data.koefs[i] = stod(line);
            i++;
            sym = input_file.get();
            if (sym == '\n' || sym == ' ')
            {
                sym = input_file.get();
            }
        }

        for (int j = 0; j < data.header.space_segments; j++)
        {
            cout << data.koefs[j] << " ";
        }
        cout << endl;

        if (i == data.header.space_segments)
        {
            cout << "Space segments correct" << endl;
        }
        else
        {
            cout << "Warning! Space_segments incorrect" << endl;
        }

        //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        
        i = 0;
        while (line != "TEMP")
        {
            getline(input_file, line);
            cout << line << endl;
            i++;
            if (i > 8)
            {
                cout << "Temp_error" << endl;
                input_file.close();
                return 0;
            }
        }

        i = 0;
        int j = 0;
        int f = 0;
        sym = 'X';
        while (sym != '/')
        {
            

            line = "";

            sym = input_file.get();
            if (sym == '\n' || sym == ' ')
            {
                sym = input_file.get();
            }

            while (sym != ' ' && sym != '\n')
            {
                line += sym;
                sym = input_file.get();
            }
            //if (stod(line) != (double)j) //плохой метод проверки - температура может совпадать с номером итерации
            //{
            //    data.temps[i] = stod(line);
            //    i++;
            //}
            //else 
            //{
            //    j++;
            //}
            if (line == "/")
            {
                break;
            }
            if (i % (data.header.space_segments + 1) == 0)
            {
                if (stod(line) != (double)j)
                {
                    cout << "Warning! Temp section iterators incorrect" << endl;
                    cout << line << endl;
                }
                else
                {
                    j++;
                }
            }
            else
            {
                data.temps[f] = stod(line);
                f++;
            }
            i++;

            
        }

        int z = 0;
        for (int k = 0; k < (data.header.space_segments * data.header.time_segments); k++)
        {
            cout << data.temps[k] << " ";
            z++;
            if (z % data.header.space_segments == 0)
            {
                cout << endl;
            }
        }
        cout << endl;

        if (j == data.header.time_segments)
        {
            cout << "Time segments correct" << endl;
        }
        else
        {
            cout << "Warning! Time_segments incorrect" << endl;
        }
        if (f / data.header.time_segments == data.header.space_segments)
        {
            cout << "Space segments correct" << endl;
        }
        else
        {
            cout << "Warning! Space_segments incorrect" << endl;
        }

    };
    input_file.close();

    return 0;
}

