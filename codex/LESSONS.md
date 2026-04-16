# LESSONS

- 2026-04-09: el repositorio no tenia inicializada la carpeta `codex/`; se crea con los ficheros base para mantener memoria de trabajo en futuras sesiones.
- 2026-04-09: `practica2/Makefile` compila `../radio.c` y `../music.c`, no unas copias locales dentro de `practica2/`. Para revisar o cambiar la logica real de la practica 2 hay que mirar los ficheros de la raiz del repo.
- 2026-04-09: `radio.c` usa una matriz de adyacencia fija `Bool relations[4096][4096]`; como `Bool` ocupa 4 bytes en este entorno, cada `Radio` reserva del orden de 67 MB. Es la principal ineficiencia estructural del proyecto.
- 2026-04-16: `practica3/` se ha dejado autocontenida para entrega con copias locales de `music.*`, `radio.*`, `types.h`, `stack.h`, `libstack.a` y `radio.txt`; el zip verificable es `practica3/P07_Pina_Fernando_Diaz_Iker.zip`.
