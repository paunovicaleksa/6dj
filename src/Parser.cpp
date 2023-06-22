#include "../inc/Parser.hpp"
#include "../inc/Image.hpp"
#include "../inc/Arithmetic.hpp"
#include "../inc/Filters.hpp"
#include "../inc/MathFunc.hpp"

static const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"filepath", required_argument, 0, 't'},
        {"add", required_argument, 0, 'a'},
        {"sub", required_argument, 0, 's'},
        {"invsub", required_argument, 0, 'i'},
        {"mul", required_argument, 0, 'm'},
        {"div", required_argument, 0, 'd'},
        {"invdiv", required_argument, 0, 'n'},
        {"pow", required_argument, 0, 'p'},
        {"log", no_argument, 0, 'l'},
        {"max", required_argument, 0, 'x'},
        {"min", required_argument, 0, 'z'},
        {"abs", no_argument, 0, 'b'},
        {"invert", no_argument, 0, 'v'},
        {"grayscale", no_argument, 0, 'g'},
        {"filter", required_argument, 0, 'f'},/* argument here is N, the size of kernel to be applied */
        {"test", no_argument, 0, 'q'}
};
/* i was very tired when i wrote this */
int Parser::parse(int32_t argc, char* argv[]){
                     
        Image* src = nullptr;
        Image *dst_no_opt = nullptr;
        Image *dst_opt = nullptr;

        int index;
        int iarg = 0;

        while(iarg != -1){
                iarg = getopt_long(argc, argv, "ht:a:s:i:m:d:n:p:lx:z:bvgf:q", longopts, &index);

                switch (iarg)
                {       
                        case 'h':{
                                printHelp();
                                return 0;
                        }
                        case 't':{
                                std::string filename = optarg;
                                src = new Image(filename.c_str());
                                if(src->load()){
                                        return 1;
                                }
                                std::uint64_t pos = filename.rfind('.');
                                std::string write_name = filename.substr(0, pos) + "_modified_no_opt" + filename.substr(pos, filename.back());

                                dst_no_opt = new Image(write_name.c_str(), src->getWidth(), src->getHeight(), src->getChannels());
                                write_name = filename.substr(0, pos) + "_modified_opt" + filename.substr(pos, filename.back());
                                dst_opt = new Image(write_name.c_str(), src->getWidth(), src->getHeight(), src->getChannels());
                                break;
                        }
                        case 'a':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::add(src, dst_no_opt, arg);
                                Arithmetic::addSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 's':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::sub(src, dst_no_opt, arg);
                                Arithmetic::subSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'i':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::invSub(src, dst_no_opt, arg);
                                Arithmetic::invSubSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'm':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::mul(src, dst_no_opt, arg);
                                Arithmetic::mulSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'd':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::div(src, dst_no_opt, arg);
                                Arithmetic::divSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'n':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                Arithmetic::invDiv(src, dst_no_opt, arg);
                                Arithmetic::invDivSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'p':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                _Float32 arg = std::stof(optarg);
                                std::cout << arg << std::endl;
                                MathFunc::pow(src, dst_no_opt, arg);
                                MathFunc::powSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'l':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                MathFunc::log(src, dst_no_opt);
                                MathFunc::logSIMD(src, dst_opt);
                                break;
                        }
                        case 'x':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                MathFunc::max(src, dst_no_opt, arg);
                                MathFunc::maxSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'z':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                MathFunc::min(src, dst_no_opt, arg);
                                MathFunc::minSIMD(src, dst_opt, arg);
                                break;
                        }
                        case 'b':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                MathFunc::abs(src, dst_no_opt);
                                MathFunc::absSIMD(src, dst_opt);
                                break;
                        }
                        case 'v':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                Filters::invert(src, dst_no_opt);
                                Filters::invertSIMD(src, dst_opt);
                                break;
                        }
                        case 'g':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                Filters::grayscale(src, dst_no_opt);
                                Filters::grayscaleSIMD(src, dst_opt);
                                break;
                        }
                        case 'f':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }

                                int32_t N = std::stoi(optarg);
                                int32_t* GX = new int32_t[N * N];
                                int32_t* GY = new int32_t[N * N];
                               
                                std::cout << "GX[N * N] = ";
                                for(int32_t i = 0; i < N * N; i++){
                                        std::cin >> GX[i];
                                }

                                std::cout << "GY[N * N] = ";
                                for(int32_t i = 0; i < N * N; i++){
                                        std::cin >> GY[i];
                                }
                                Filters::applyKernel(src, dst_no_opt, GX, GY, N);                                  
                                Filters::applyOptimizedKernel(src, dst_opt, GX, GY, N); 

                                delete[] GX;
                                delete[] GY;
                                break;
                        }
                        case 'q':{
                                if(src == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                test("all", src);
                                break;
                        }
                }
        }
        
        if(src){
                dst_no_opt->write();
                dst_opt->write();
                delete src;
                delete dst_no_opt;
                delete dst_opt;
        }

        return 0;
}

void Parser::printHelp(){
        std::cerr << "general usage: --filepath [path-to-file] [options]" << std::endl;
        std::cerr <<"options: " << std::endl;
        std::cerr <<"\t--add -a\t[arg]\tadd arg to color compotents of input file" << std::endl;
        std::cerr <<"\t--sub -s\t[arg]\tsub arg from color compotents of input file" << std::endl;
        std::cerr <<"\t--invSub -i\t[arg]\tsub color compotents of input file from arg" << std::endl;
        std::cerr <<"\t--mul -m\t[arg]\tmul arg with color compotents of input file" << std::endl;
        std::cerr <<"\t--div -d\t[arg]\tdiv color compotents of input file with arg" << std::endl;
        std::cerr <<"\t--invdiv -n\t[arg]\tdiv arg with color components" << std::endl;
        std::cerr <<"\t--pow -p\t[arg]\tcalculate color component to the power of arg" << std::endl;
        std::cerr <<"\t--log -l\t\tcalculate natural logarithm of color component" << std::endl;
        std::cerr <<"\t--max -x\t[arg]\tset color components of output to max(arg, color_component)" << std::endl;
        std::cerr <<"\t--min -z\t[arg]\tset color components of output to min(arg, color_component)" << std::endl;
        std::cerr <<"\t--abs -b\t\tset color components to abs(color_component)" << std::endl;
        std::cerr <<"\t--invert -v\t\tinvert every color component by calculating 255 - color_component" << std::endl;
        std::cerr <<"\t--grayscale -g\t\tsets every color component to the average of neighboring components" << std::endl;
        std::cerr <<"\t--filter -f\t[arg] [1D input arrays]\tapply filter to the color component and its neighbors" << std::endl;
        std::cerr <<"\t--test -q\t\trun tests for everything" << std::endl;
        std::cerr <<"\t--help -h\t\tprint this help and exit" << std::endl;
}


void Parser::test(std::string test_name, Image* src){

        Image *dst_no_opt = new Image("test_no_opt", src->getWidth(), src->getHeight(), src->getChannels());
        Image *dst_opt = new Image("test_opt", src->getWidth(), src->getHeight(), src->getChannels());

        Arithmetic::add(src, dst_no_opt, 100);
        Arithmetic::addSIMD(src, dst_opt, 100);
        
        Arithmetic::sub(src, dst_no_opt, 100);
        Arithmetic::subSIMD(src, dst_opt, 100);

        Arithmetic::mul(src, dst_no_opt, 2);
        Arithmetic::mulSIMD(src, dst_opt, 2);
        
        Arithmetic::div(src, dst_no_opt, 2);
        Arithmetic::divSIMD(src, dst_opt, 2);

        Arithmetic::invDiv(src, dst_no_opt, 250);
        Arithmetic::invDivSIMD(src, dst_opt, 250);
        
        MathFunc::pow(src, dst_no_opt, 2);
        MathFunc::powSIMD(src, dst_opt, 2);

        MathFunc::log(src, dst_no_opt);
        MathFunc::logSIMD(src, dst_opt);

        MathFunc::max(src, dst_no_opt, 128);
        MathFunc::maxSIMD(src, dst_opt, 128);

        MathFunc::min(src, dst_no_opt, 128);
        MathFunc::minSIMD(src, dst_opt, 128);

        MathFunc::abs(src, dst_no_opt);
        MathFunc::absSIMD(src, dst_opt);

        Filters::grayscale(src, dst_no_opt);
        Filters::grayscaleSIMD(src, dst_opt);

        Filters::invert(src, dst_no_opt);
        Filters::invertSIMD(src, dst_opt);

        int32_t N = 3;
        int32_t GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
        int32_t GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1}; 
        
        Filters::applyKernel(src, dst_no_opt, GX, GY, N);
        Filters::applyOptimizedKernel(src, dst_opt, GX, GY, N);

        delete dst_no_opt;
        delete dst_opt;
}


