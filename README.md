<h1 align="center">MiniCompiler-Kit</h1>
<p align="center">A lexical and syntactic analyzer kit for MINI-language, implemented in <strong>C</strong>.</p>

<p align="center">
  <img src="https://img.shields.io/badge/C-Language-00599C?logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/Parser-Recursive_Descent-7B68EE" alt="Recursive Descent Parser">
  <img src="https://img.shields.io/badge/Build-Makefile-427819?logo=gnu&logoColor=white" alt="Makefile">
</p>

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
### Clean the compiler
To clean the generated files, run:

```zsh
make clean
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
