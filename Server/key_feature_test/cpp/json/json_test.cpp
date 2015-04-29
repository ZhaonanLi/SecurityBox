// This is a jsoncpp test file.
#include <iostream>
#include <string>

#include <json/json.h>

int main()
{
    std::string json_string = "{\"username\":\"tom\",\"password\":\"12345\"}";
    std::cout << "The json string = " << json_string << std::endl;

    Json::Reader reader;
    Json::Value value;

    bool is_succ = reader.parse(json_string, value);
    if (is_succ)
    {
        std::cout << "Json parse successfully." << std::endl;

        // Show the Json Structure.
        std::cout << "username = " << value["username"];
        std::cout << "password = " << value["password"];
    }
    else
    {
        std::cout << "Json parse unsuccessfully." << std::endl;
    }
    return 0;
}
