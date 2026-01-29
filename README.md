Instalar liberia GPIOD
```
sudo apt update
sudo apt install gpiod libgpiod-dev
```

Validar estados del GPIO

pinctrl get 22 # ve estado del GPIO

pinctrl set 22 pu # setea el estado del GPIO a up
pinctrl set 22 pd # setea el estado del GPIO a down

