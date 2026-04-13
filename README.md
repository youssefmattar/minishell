# minishell
minishell is simple linux shell that can only execute commands running as a user program no tty or pty .

## 🚀 Features
* **foreground/background processes:** support running a process in back ground by adding  '&' and getting it back by fg
* **support one pipe | :** you can use grep like any other shell but no colors
* **support >, >>, <** you can get input from files or output to files instead of the shell if you want

## 🛠️ Tech Stack
* **Language:** C/C++
* **Tools:** SFML, Git, G++

## 📦 Installation
How to get the environment ready.

```bash
#install SFML (this command should work on ubuntu/debian systems)
sudo apt update
sudo apt install build-essential
sudo apt install libsfml-dev

# Clone the repository
git clone [https://github.com/youssefmattar/minishell.git](https://github.com/youssefmattar/minishell.git)

# Navigate to the directory
cd minishell

#compile the main.cpp
g++ main.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system