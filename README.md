# MiniCompiler-Kit
A lexical and syntactic analyzer kit for MINI-language, implemented in C. This educational project demostrates the core principle of compliers by transforming source code into a token stream and verify its syntactic structrure using recursive desent parsing.

---
## 🛠️ Build & Usage

### Prerequirements
- A C compiler. (eg., gcc, clang)

### Complitation
Clone the repository and build the project using the provided Makefile:

```zsh
git clone git@github.com:NoGamble/MiniCompiler-Kit.git
cd MiniComplier-Kit
make
```

This will generate an exucatable named complier.
### Running the complier
Execute the program by providing the input source file and the desired base name for the output files.

```zsh
# Usage: ./compiler <input_file.mini> <output_basename>
./compiler test/test_1.mini output/mini_1
```

## 📤 Output Files
After a successful run, the following files will be generated in the specified output directory (e.g., output/):

#### Output Files Description / 输出文件说明

| File / 文件 | Description / 描述 |
| :--- | :--- |
| `*.dyd` | **Token Stream / 单词序列** <br> Contains all recognized tokens. Each line represents one token in the format `(Category_Code, Attribute_Value)`. <br> 包含所有识别出的单词符号。每行一个二元式，格式为 `(种别码, 单词自身或属性值)`。 |
| `*.err` | **Error Messages / 错误信息** <br> Records all lexical and syntactic errors found during analysis. The format for each error is: `***LINE: <Line Number> <Error Description>`. <br> 记录分析过程中发现的所有词法和语法错误。每条错误信息的格式为: `***LINE: <行号> <错误描述>`。 |
| `*.var` | **Variable Symbol Table / 变量名表** <br> Lists all declared variables with their attributes (e.g., name, enclosing procedure, type, level, address). <br> 列出所有声明的变量及其属性（如变量名、所属过程、类型、层次、地址）。 |
| `*.pro` | **Procedure Symbol Table / 过程名表** <br> Lists all declared procedures/functions with their properties (e.g., name, type, level, variable range). <br> 列出所有声明的过程/函数及其属性（如过程名、类型、层次、变量区间）。 |

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details. 