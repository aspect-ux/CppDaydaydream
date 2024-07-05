#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> // basic file logging
#include <spdlog/sinks/rotating_file_sink.h> // support for rotating file logging
#include <yaml-cpp/yaml.h>
// Serialization序列化
// ref: 
// 1. https://github.com/jbeder/yaml-cpp
// 使用yaml文件读取保存配置,.yaml/.yml更易阅读
// 大小写敏感，只能用空格不能用tab

// [Data type]
// 1. 对象(key-value)
// 2. 数组(sequence)
// 3. 标量(scalars): 单个不可再分的值

// [Yaml API]
// 1. YAML::Emitter: 生成YAML格式文件
// 2. YAML::Node: 存储解析YAML文件后的中间数据 key-value类型

struct Vec3 { double x, y, z; /* etc - make sure you have overloaded operator== */ };
namespace YAML {
    template<>
    struct convert<Vec3> {
        static Node encode(const Vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            rhs.z = node[2].as<double>();
            return true;
        }
    };
}
int main()
{
    // Introduction
    {
        // 1. load file from .yaml to node
        YAML::Node config = YAML::LoadFile("config.yaml");

        // 2. node is intermediate data, you can read & write
        // read
        if (config["PresetDirectory"]) {
            std::cout << "preset dir: " << config["PresetDirectory"].as<std::string>();
        }
        // write: original value is `zhangsan`, then we change it
        config["username"] = "张三";

        // 3. write back to file
        std::ofstream fout("config.yaml");
        fout << config;
    }
    // Parsing and Node Editing 解析和编辑Node
    {
        // yaml文件中包括根节点root在内的所有节点都可以用YAML::Node表示
        // 1. 下面的序列[1,2,3]直接读取到nodes
        YAML::Node nodes = YAML::Load("[1, 2, 3]");
        assert(nodes.Type() == YAML::NodeType::Sequence);
        assert(nodes.IsSequence());  // a shortcut!

        std::cout << std::endl;
        // [stl-like] 我们可以像stl vector一样使用nodes
        for (YAML::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
            std::cout << it->as<int>() << "\n";
        }//...

        // 2. query node type
        /*switch (nodes.Type()) {
        case Null: // ...
        case Scalar: // ...
        case Sequence: // ...
        case Map: // ...
        case Undefined: // ...
        }*/

        // 3. query doesnot create them automatically
        YAML::Node node = YAML::Load("{name: Brewers, city: Milwaukee}");
        if (node["name"]) {
            std::cout << node["name"].as<std::string>() << "\n";
        }
        if (node["mascot"]) {
            std::cout << node["mascot"].as<std::string>() << "\n";
        }
        assert(node.size() == 2); // the previous call didn't create a node
    }

    // Build Node: 手动构建节点
    {
        YAML::Node node;  // starts out as null
        node["key"] = "value";  // it now is a map node
        node["seq"].push_back("first element");  // node["seq"] automatically becomes a sequence
        node["seq"].push_back("second element");

        node["mirror"] = node["seq"][0];  // this creates an alias
        node["seq"][0] = "1st element";  // this also changes node["mirror"]
        node["mirror"] = "element #1";  // and this changes node["seq"][0] - they're really the "same" node

        node["self"] = node;  // you can even create self-aliases
        node[node["mirror"]] = node["seq"];  // and strange loops :)
    }

    // Sequence to maps: 序列/数组转map
    {
        // 可以根据下标更改，没有声明key,默认0-n
        YAML::Node node = YAML::Load("[1, 2, 3]");
        node[1] = 5;  // still a sequence, [1, 5, 3]
        node.push_back(-3); // still a sequence, [1, 5, 3, -3]
        node["key"] = "value"; // now it's a map! {0: 1, 1: 5, 2: 3, 3: -3, key: value}
    }

    // Converting
    {
        YAML::Node node = YAML::Load("{pi: 3.14159, [0, 1]: integers}");

        // this needs the conversion from Node to double
        double pi = node["pi"].as<double>();

        // this needs the conversion from double to Node
        node["e"] = 2.71828;

        // this needs the conversion from Node to std::vector<int> (*not* the other way around!)
        std::vector<int> v;
        v.push_back(0);
        v.push_back(1);
        std::string str = node[v].as<std::string>();

        // convert implicitly with your own data type
        YAML::Node node = YAML::Load("start: [1, 3, 0]");
        Vec3 v = node["start"].as<Vec3>();
        //node["end"] = Vec3(2, -1, 0); //operation == overload
    }
    

    return 0;
}