#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "lib/fifo_map.hpp"

//a workaround to give to use fifo_map as map, we are just ignoring the 'less' compare
//https://github.com/nlohmann/json/issues/485
template<class K, class V, class dummy_compare, class A>
using workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using json = nlohmann::basic_json<workaround_fifo_map>;

//import from error.cpp
int bytecode_error(uint8_t bytecode);
int key_error();
int out_of_range_error(uint8_t bytecode);
int eof_error(char footer[5]);

//import from main.cpp
std::string to_hex_string(uint64_t q);
std::string to_hex_string(std::vector <uint8_t> a);
extern std::ifstream input;
extern std::ofstream debug;
extern json debug_js;

//import from RTON_number.cpp
std::vector <uint8_t> int2unsigned_RTON_num(uint64_t q);
uint64_t unsigned_RTON_num2int(std::vector <uint8_t> q);

std::vector <std::string> stack_0x91;
std::vector <std::string> stack_0x93;

json read_RTON();

template <class T>
T read(){
    T res;
    input.read(reinterpret_cast <char*> (&res), sizeof res);
    return res;
}

std::vector <uint8_t> read_RTON_num(){
    std::vector <uint8_t> RTON_num;
    uint8_t sub_num = 0x80;
    while(sub_num > 0x7f){
        sub_num = read<uint8_t>();
        RTON_num.push_back(sub_num);
    }
    return RTON_num;
}

json read_RTON_block(){
    json res;

    uint8_t bytecode = read<uint8_t>();
    debug_js["RTON stats"]["List of bytecodes"][to_hex_string((uint64_t) input.tellg() - 1)] = to_hex_string(bytecode);

    switch (bytecode){
        //false
        case 0x0:{
            res.push_back(false);
            break;
        }
        //true
        case 0x1:{
            res.push_back(true);
            break;
        }
        //int8_t
        case 0x8:{
            res.push_back(read<int8_t>());
            break;
        }
        //0???
        case 0x9:{
            res.push_back(0);
            break;
        }
        //uint8_t
        case 0xa:{
            res.push_back(read<uint8_t>());
            break;
        }
        //0???
        case 0xb:{
            res.push_back(0);
            break;
        }
        //int16_t
        case 0x10:{
            res.push_back(read<int16_t>());
            break;
        }
        //0???
        case 0x11:{
            res.push_back(0);
            break;
        }
        //uint16_t
        case 0x12:{
            res.push_back(read<uint16_t>());
            break;
        }
        //0???
        case 0x13:{
            res.push_back(0);
            break;
        }
        //int32_t
        case 0x20:{
            res.push_back(read<int32_t>());
            break;
        }
        //0???
        case 0x21:{
            res.push_back(0);
            break;
        }
        //float32
        case 0x22:{
            res.push_back(read<float>());
            break;
        }
        //0.0???
        case 0x23:{
            res.push_back(0.0);
            break;
        }
        //unsigned RTON number
        case 0x24:{
            res.push_back(unsigned_RTON_num2int(read_RTON_num()));
            break;
        }
        //RTON number
        case 0x25:{
            int64_t num = unsigned_RTON_num2int(read_RTON_num());
            if (num % 2) num = -(num + 1);
            num /= 2;

            res.push_back(num);
            break;
        }
        //uint32_t
        case 0x26:{
            res.push_back(read<uint32_t>());
            break;
        }
        //0???
        case 0x27:{
            res.push_back(0);
            break;
        }
        //unsigned RTON number???
        case 0x28:{
            res.push_back(unsigned_RTON_num2int(read_RTON_num()));
            break;
        }
        //RTON number???
        case 0x29:{
            int64_t num = unsigned_RTON_num2int(read_RTON_num());
            if (num % 2) num = -(num + 1);
            num /= 2;

            res.push_back(num);
            break;
        }
        //int64_t
        case 0x40:{
            res.push_back(read<int64_t>());
            break;
        }
        //0???
        case 0x41:{
            res.push_back(0);
            break;
        }
        //float64
        case 0x42:{
            res.push_back(read<double>());
            break;
        }
        //0.0???
        case 0x43:{
            res.push_back(0.0);
            break;
        }
        //unsigned RTON number???
        case 0x44:{
            res.push_back(unsigned_RTON_num2int(read_RTON_num()));
            break;
        }
        //RTON number???
        case 0x45:{
            int64_t num = unsigned_RTON_num2int(read_RTON_num());
            if (num % 2) num = -(num + 1);
            num /= 2;

            res.push_back(num);
            break;
        }
        //uint64_t
        case 0x46:{
            res.push_back(read<uint64_t>());
            break;
        }
        //0???
        case 0x47:{
            res.push_back(0);
            break;
        }
        //unsigned RTON number???
        case 0x48:{
            res.push_back(unsigned_RTON_num2int(read_RTON_num()));
            break;
        }
        //RTON number???
        case 0x49:{
            int64_t num = unsigned_RTON_num2int(read_RTON_num());
            if (num % 2) num = -(num + 1);
            num /= 2;

            res.push_back(num);
            break;
        }
        //string
        case 0x81:{
            uint64_t buffer = unsigned_RTON_num2int(read_RTON_num());

            char temp[buffer + 1];
            input.read(temp, buffer);
            temp[buffer] = 0;

            res.push_back(std::string(temp));
            break;
        }
        //utf-8 string
        case 0x82:{
            uint64_t buffer = unsigned_RTON_num2int(read_RTON_num());
            buffer = unsigned_RTON_num2int(read_RTON_num());

            char s[buffer + 1];
            input.read(s, buffer);
            s[buffer] = 0;

            res.push_back(std::string(s));
            break;
        }
        //RTID
        case 0x83:{
            uint8_t subset = read<uint8_t>();

            if (subset == 0x3){
                uint64_t s1_buffer = unsigned_RTON_num2int(read_RTON_num());
                s1_buffer = unsigned_RTON_num2int(read_RTON_num());

                char s1[s1_buffer + 1];
                input.read(s1, s1_buffer);
                s1[s1_buffer] = 0;

                uint64_t s2_buffer = unsigned_RTON_num2int(read_RTON_num());
                s2_buffer = unsigned_RTON_num2int(read_RTON_num());

                char s2[s2_buffer + 1];
                input.read(s2, s2_buffer);
                s2[s2_buffer] = 0;

                res.push_back(std::string("RTID(") + s2 + '@' + s1 + ')');
            }
            else if (subset == 0x2){
                uint64_t buffer = unsigned_RTON_num2int(read_RTON_num());
                buffer = unsigned_RTON_num2int(read_RTON_num());

                char s[buffer + 1];
                input.read(s, buffer);
                s[buffer] = 0;

                uint64_t second_uid = unsigned_RTON_num2int(read_RTON_num());
                uint64_t first_uid = unsigned_RTON_num2int(read_RTON_num());
                uint32_t third_uid = read<uint32_t>();

                std::stringstream ss;
                std::string uid;
                ss << std::dec << first_uid << '.' << second_uid << '.' << std::hex << third_uid;
                ss >> uid;

                res.push_back(std::string("RTID(") + uid + '@' + s + ')');
            }
            else throw bytecode_error(subset);
            break;
        }
        //null
        case 0x84:{
            res.push_back(nullptr);
            break;
        }
        //sub-object
        case 0x85:{
            res.push_back(read_RTON());
            break;
        }
        //array
        case 0x86:{
            uint8_t arr_begin = read<uint8_t>();

            if (arr_begin == 0xfd){
                size_t arr_size = unsigned_RTON_num2int(read_RTON_num());

                json arr = json::array();
                for (size_t i = 0; i < arr_size; ++i) arr.push_back(read_RTON_block()[0]);

                res.push_back(arr);

                uint8_t arr_end = read<uint8_t>();
                if (arr_end != 0xfe) throw bytecode_error(arr_end);
            }
            else throw bytecode_error(arr_begin);
            break;
        }
        //cached string
        case 0x90:{
            uint64_t buffer = unsigned_RTON_num2int(read_RTON_num());

            char temp[buffer + 1];
            input.read(temp, buffer);
            temp[buffer] = 0;

            debug_js["RTON stats"]["0x91 stack"][to_hex_string(int2unsigned_RTON_num(stack_0x91.size()))] = std::string(temp);
            stack_0x91.push_back(temp);

            res.push_back(stack_0x91[stack_0x91.size() - 1]);
            break;
        }
        //recall
        case 0x91:{
            try{
                res.push_back(stack_0x91.at(unsigned_RTON_num2int(read_RTON_num())));
            }
            catch(const std::out_of_range &oor){
                throw out_of_range_error(bytecode);
            }
            break;
        }
        //cached utf-8 string
        case 0x92:{
            uint64_t buffer = unsigned_RTON_num2int(read_RTON_num());
            buffer = unsigned_RTON_num2int(read_RTON_num());

            char temp[buffer + 1];
            input.read(temp, buffer);
            temp[buffer] = 0;

            debug_js["RTON stats"]["0x93 stack"][to_hex_string(int2unsigned_RTON_num(stack_0x93.size()))] = std::string(temp);
            stack_0x93.push_back(temp);

            res.push_back(stack_0x93[stack_0x93.size() - 1]);
            break;
        }
        //recall
        case 0x93:{
            try{
                res.push_back(stack_0x93.at(unsigned_RTON_num2int(read_RTON_num())));
            }
            catch(const std::out_of_range &oor){
                throw out_of_range_error(bytecode);
            }
            break;
        }
        //end of object
        case 0xFF:{
            break;
        }
        //else just exit error
        default:{
            throw bytecode_error(bytecode);
            break;
        }
    }
    return res;
}

json read_RTON(){
    json res;
    while(true){
        std::string key;
        json js_key;
        js_key = read_RTON_block();
        if (js_key.size() == 0) return res;
        else{
            if (!js_key[0].is_string()) throw key_error();
            key = js_key[0];
        }

        //prevent push entire array lol
        json value = read_RTON_block()[0];
        res[key] = value;
    }
}

json json_decode(){
    stack_0x91.clear();
    stack_0x93.clear();

    input.seekg((uint64_t) input.tellg() + 4); //skip RTON
    uint32_t RTON_ver = read<uint32_t>();
    debug_js["RTON stats"]["RTON version"] = RTON_ver;

    json js;
    js = read_RTON();

    if (input.peek() == EOF) std::clog << "Missing \"DONE\" at EOF?" << std::endl;
    else{
        char footer[5];
        input.read(footer, 4);
        footer[4] = 0;

        if (strcmp(footer, "DONE") != 0){
            input.seekg((uint8_t) input.tellg() - 3);
            throw eof_error(footer);
        }
    }

    return js;
}
