#include "../inc/Parser.hpp"


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
        {"test", required_argument, 0, 'q'}
};
/* i was very tired when i wrote this */
int Parser::parse(int32_t argc, char* argv[]){
                     
        Filter *f = nullptr;

        int index;
        int iarg = 0;

        while(iarg != -1){
                iarg = getopt_long(argc, argv, "ht:a:s:i:m:d:n:p:lx:z:bvgf:q:", longopts, &index);

                switch (iarg)
                {       
                        case 'h':{
                                printHelp();
                                return 0;
                        }
                        case 't':{
                                std::string filename = optarg;
                                f = new Filter(filename.c_str());
                                if(f->load()){
                                        return 1;
                                }
                                break;
                        }
                        case 'a':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->addSIMD(arg);
                                break;
                        }
                        case 's':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->subSIMD(arg);
                                break;
                        }
                        case 'i':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->invSubSIMD(arg);
                                break;
                        }
                        case 'm':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->mulSIMD(arg);
                                break;
                        }
                        case 'd':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->divSIMD(arg);
                                break;
                        }
                        case 'n':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->invDivSIMD(arg);
                                break;
                        }
                        case 'p':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->powSIMD(arg);
                                break;
                        }
                        case 'l':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                f->logSIMD();
                                break;
                        }
                        case 'x':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->maxSIMD(arg);
                                break;
                        }
                        case 'z':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                uint8_t arg = std::stoi(optarg);
                                f->minSIMD(arg);
                                break;
                        }
                        case 'b':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                f->absSIMD();
                                break;
                        }
                        case 'v':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                f->invertSIMD();
                                break;
                        }
                        case 'g':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                f->grayscaleSIMD();
                                break;
                        }
                        case 'f':{
                                if(f == nullptr){
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

                                f->filterOptimized(GX, GY);
                                delete[] GX;
                                delete[] GY;
                                break;
                        }
                        case 'q':{
                                if(f == nullptr){
                                        printHelp();
                                        return 1;
                                }
                                std::string test_name = optarg;
                                test(test_name, f);
                                break;
                        }
                }
        }
        f->write();
        delete f;
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
        std::cerr <<"\t--test -q\t[arg]\trun tests for function named arg, \"all\" for testing everything" << std::endl;
        std::cerr <<"\t--help -h\t\tprint this help and exit" << std::endl;
}

void test_all(Filter *f){
        f->add(100);
        f->addSIMD(100);

        f->sub(100);
        f->subSIMD(100);

        f->mul(2);
        f->mulSIMD(2);

        f->div(2);
        f->divSIMD(2);

        f->invDiv(3);
        f->invDivSIMD(3);

        f->pow(2);
        f->powSIMD(2);

        f->log();
        f->logSIMD();

        f->max(100);
        f->maxSIMD(100);

        f->min(100);
        f->minSIMD(100);

        f->abs();
        f->absSIMD();

        f->invert();
        f->invertSIMD();

        f->grayscale();
        f->grayscaleSIMD();

        int32_t N = 3;
        int32_t GX[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
        int32_t GY[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 }; 
        
        f->filter(GX, GY);
        f->filterOptimized(GX, GY);
}

void Parser::test(std::string test_name, Filter* f){
        test_all(f);
}


