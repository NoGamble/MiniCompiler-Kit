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
# Usage: ./bin/MiniCompiler.o <input_file.mini> <output_basename>
./bin/MiniCompiler.o test/test_1.mini output/mini_1
```

## 📤 Output Files
After a successful run, the following files will be generated in the specified output directory (e.g., output/):

#### Output Files Description / 输出文件说明

| File | Description |
| :--- | :--- |
| `*.dyd` | **Token Stream** <br> Contains all recognized tokens. Each line represents one token in the format `(Category_Code, Attribute_Value)`. <br> |
| `*.err` | **Error Messages** <br> Records all lexical and syntactic errors found during analysis. The format for each error is: `***LINE: <Line Number> <Error Description>`. <br>  |
| `*.var` | **Variable Symbol Table** <br> Lists all declared variables with their attributes (e.g., name, enclosing procedure, type, level, address). <br>  |
| `*.pro` | **Procedure Symbol Table** <br> Lists all declared procedures/functions with their properties (e.g., name, type, level, variable range). <br>  |

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details. 