#include <iostream>
#include <thread>

void manage_memory(long long int** mem, size_t& n, double factor = 1.);

int main(int argc, char** argv)
{
    std::cout << "Be warned...!  Memory hogger is warming up..." << std::endl;

    double mem_size_to_hog_gb = 10.;
    if (argc > 1)
        mem_size_to_hog_gb = std::atof(argv[1]);

    mem_size_to_hog_gb *= (1024 * 1024 * 1024);

    size_t N = mem_size_to_hog_gb / sizeof(long long int);
    long long int* memory_yard = nullptr;

    manage_memory(&memory_yard, N);
    
    std::string user_input{ "k" };
    while (user_input.compare("r") != 0)
    {
        std::cout << "Holing onto " << N << " values (aka " << N * sizeof(long long int) / 1024. / 1024. / 1024. << " GB) for you..." << std::endl;
        std::cout << "waiting for your input to react (r: release, {num}: change by multiplying with num) :: ";
        std::cin >> user_input;

        char* p;
        double factor = strtod(user_input.c_str(), &p);
        if (*p) {
            // conversion failed because the input wasn't a number
            continue;
        }
        else {
            // use converted
            manage_memory(&memory_yard, N, factor);
        }
    }

    if (memory_yard)
    {
        delete[] memory_yard;
        memory_yard = nullptr;
    }

    std::cout << "\n(machine sighs in relief)\n" << std::endl;
    return EXIT_SUCCESS;
}

void manage_memory(long long int** mem, size_t& n, double factor)
{
    n *= factor;
    while (*mem == nullptr)
    {
        try
        {
            *mem = new long long int[n];
        }
        catch (const std::exception& e)
        {
            std::cout << "Reducing number of elements by a quarter >>" << std::endl;
            n *= 0.75;
        }
    }

    // don't be ridiculous, leave some space for normal ops
    if (*mem)
    {
        delete[] *mem;
        *mem = nullptr;
        n *= 0.9;
        *mem = new long long int[n];
    }
}
