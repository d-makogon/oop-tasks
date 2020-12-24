# Сборка и запуск

```bash
mkdir build && cd build
cmake ..
cmake --build . --target app
cd app
./app [options]
```

# Аргументы

* -c --count - число раундов (по умолчанию: 1)
* -f --first - тип первого игрока: console, random или optimal (по умолчанию: random)
* -s --second - тип второго игрока: console, random или optimal (по умолчанию: random)