# 🟢 Tactile Switch con libgpiod en Raspberry Pi (con HAT SPI)

Este proyecto demuestra cómo leer **switch táctiles (4 patas)** usando **libgpiod** en Raspberry Pi

---


## 📦 Instalación de libgpiod

```bash
sudo apt update
sudo apt install gpiod libgpiod-dev
```

Verificar instalación:

```bash
gpiodetect
```

---

## 🛠️ Compilación

```bash
gcc main.c -o main -lgpiod
```

Ejecutar:

```bash
./main
```

---

## 🔌 Cableado del switch (pull-up recomendado)

Configuración usada en este proyecto:

| GPIO | Pin físico |
|-----|------------|
| GPIO21 | Pin 40 |
| GPIO26 | Pin 37 |
| GND | Pin 39 |

**Cableado (para cada botón):**
- Una pata del botón → GPIO
- La pata opuesta → **GND**
- NO conectar a 3.3V

Con **pull-up**, el comportamiento es:
- reposo → `1`
- presionado → `0`

---

## 🧠 ¿Qué es pull-up / pull-down?

Los GPIO pueden quedar en estado **flotante** si no se define su nivel eléctrico.

### Pull-up
- El pin se mantiene en `1` (3.3V) internamente
- El botón lo lleva a `0` al conectarlo a GND
- ✅ Ideal cuando solo tienes GND disponible

### Pull-down
- El pin se mantiene en `0`
- El botón debe conectarse a **3.3V**
- ❌ No usable si no tienes acceso físico a 3.3V

---

## 🔍 Diagnóstico de GPIO

Para ver el estado real de un GPIO:

```bash
pinctrl get 21
```

Ejemplo:

```
21: ip pu | hi
```

Significado:
- `ip` → input
- `pu` → pull-up
- `hi` → nivel lógico 1

Cambiar el estado manualmente (temporal):

```bash
pinctrl set 21 pu
pinctrl set 21 pd
```

⚠️ Esto **NO persiste** tras reiniciar.

---

## ❗ Problema común con pantallas SPI (HAT)

Cuando se conecta una **pantalla SPI tipo Waveshare / genérica**, el overlay:
- reconfigura varios GPIO
- los deja en **pull-down por defecto**
- libgpiod **NO cambia pull-up/pull-down**

### Síntoma típico
- El programa termina inmediatamente
- Los GPIO se leen siempre en `0`
- Ejecutar `pinctrl set XX pu` hace que funcione temporalmente

👉 **No es un bug del código**  
👉 Es un conflicto entre el HAT y la configuración eléctrica del GPIO

---

## ✅ Solución recomendada (persistente)

Configurar los GPIO en **/boot/firmware/config.txt**.

Editar:

```bash
sudo nano /boot/firmware/config.txt
```

Agregar al final:

```ini
# Tactile switches (libgpiod)
gpio=21=pu
gpio=26=pu
```

Reiniciar:

```bash
sudo reboot
```

Verificar:

```bash
pinctrl get 21
pinctrl get 26
```

Debe indicar:

```
ip pu | hi
```

---

## 🧪 Alternativa (solo pruebas)

```bash
sudo pinctrl set 21 pu
sudo pinctrl set 26 pu
./main
```

---

## 🧩 Nota sobre libgpiod

`libgpiod` **no configura pull-up/pull-down** en versiones 1.x.  
Solo solicita dirección del pin:

```c
gpiod_line_request_input(line, "test_gpio");
```

Por eso el estado depende del kernel, overlays y HAT conectados.

---

## 📚 Lecturas recomendadas

- https://wiki.friendlyelec.com/wiki/index.php/GPIO_Programming:_Using_the_libgpiod_Library
- https://libgpiod.readthedocs.io/en/latest/

---
