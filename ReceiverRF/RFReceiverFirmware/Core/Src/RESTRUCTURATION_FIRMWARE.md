# Proposition de restructuration du firmware

## Objectif

Separer clairement :

- le code genere par STM32CubeMX
- les pilotes materiels du projet
- les services applicatifs
- l'etat global de l'application
- le point d'entree minimal du firmware

L'objectif n'est pas seulement de deplacer des fichiers, mais de rendre le projet maintenable, testable et lisible, sans se mettre en conflit avec la regeneration STM32CubeMX.

## Contrainte STM32CubeMX

STM32CubeMX genere et regenere principalement :

- `Core/Inc`
- `Core/Src`
- `Drivers/`
- `Core/Src/main.c`
- `Core/Src/stm32f1xx_it.c`
- les fonctions `MX_*_Init()`
- les zones `USER CODE BEGIN` / `USER CODE END`

En pratique, cela change la strategie de restructuration :

- il ne faut pas deplacer les fichiers generes par CubeMX hors de leur emplacement attendu
- il ne faut pas renommer `main.c`, `main.h`, `gpio.c`, `spi.c`, `tim.c`, `i2c.c`, `usart.c`, `stm32f1xx_it.c`
- il faut limiter les modifications dans les fichiers generes aux zones `USER CODE`
- il vaut mieux ajouter des sous-dossiers sous `Core/Inc` et `Core/Src` plutot que sortir le code du dossier `Core`
- il faut eviter une structure qui impose de reconfigurer souvent les include paths dans `.cproject`

## Problemes constates dans la structure actuelle

- `Core/Inc` et `Core/Src` contiennent a la fois du code CubeMX et du code applicatif.
- `GAUL_utilitie` melange etat global, logique d'affichage et fichiers vides.
- un fichier source `.c` existe dans `Core/Inc/GAUL_utilitie`.
- `main.c` contient l'initialisation bas niveau, la logique radio, la gestion des boutons et la logique d'affichage.
- les noms ne suivent pas une convention stable : `Pulse_pin`, `gestion_lcd`, `utilitie`, `utility`.
- les includes ne sont pas coherents sur la casse : `GAUL_drivers/...` vs `Gaul_drivers/...`.

## Structure cible recommandee compatible CubeMX

```text
RFReceiverFirmware/
  Core/
    Inc/
      main.h
      gpio.h
      i2c.h
      spi.h
      tim.h
      usart.h
      stm32f1xx_it.h
      app/
        app.h
        app_state.h
        app_config.h
      app/services/
        radio_service.h
        ui_service.h
        input_service.h
      bsp/
        board_init.h
      gaul/drivers/
        rfm22.h
        i2c_lcd.h
        pulse_pin.h
        buzzer.h
        mlx90393.h
        timer_utils.h
    Src/
      main.c
      gpio.c
      i2c.c
      spi.c
      tim.c
      usart.c
      stm32f1xx_it.c
      stm32f1xx_hal_msp.c
      syscalls.c
      sysmem.c
      system_stm32f1xx.c
      app/
        app.c
        app_state.c
      app/services/
        radio_service.c
        ui_service.c
        input_service.c
      bsp/
        board_init.c
      gaul/drivers/
        rfm22.c
        i2c_lcd.c
        pulse_pin.c
        buzzer.c
        mlx90393.c
        timer_utils.c

  Drivers/
    CMSIS/
    STM32F1xx_HAL_Driver/
```

Cette structure garde tout le code utilisateur dans `Core`, ce qui reste compatible avec les include paths actuels du projet STM32CubeIDE/CubeMX.

## Regles de decoupage

### 1. `Core/`

`Core` reste la racine du code compile par CubeMX et STM32CubeIDE.

- les fichiers generes restent a leur place
- le code applicatif est ajoute dans des sous-dossiers dedies sous `Core/Inc` et `Core/Src`
- `main.c` doit devenir un point d'entree minimal
- aucune logique metier durable ne devrait rester dans `Core/Src/main.c` hors zones `USER CODE`

Contenu vise pour `main.c` :

- `HAL_Init()`
- `SystemClock_Config()`
- `MX_*_Init()`
- `Board_Init()`
- `App_Init()`
- boucle infinie appelant `App_Run()`

Important : `main.c` doit rester dans `Core/Src/main.c`, car CubeMX le gere.

### 2. `Core/Inc/gaul/drivers` et `Core/Src/gaul/drivers`

Place ici les pilotes metier du projet, tout en restant sous `Core` pour la compatibilite CubeMX.

- un module = un composant ou une abstraction materielle
- pas de variables globales applicatives
- pas d'appel direct a `print_menu()` ou a une logique ecran metier

Exemples :

- `rfm22.[hc]`
- `i2c_lcd.[hc]`
- `pulse_pin.[hc]`
- `buzzer.[hc]`
- `mlx90393.[hc]`

### 3. `Core/Inc/app` et `Core/Src/app`

Place ici la logique metier du recepteur RF.

- traitement des interruptions radio
- gestion des boutons
- maintien du mode RX
- mise a jour de l'ecran
- calcul du signal et comportement buzzer

Cette couche depend des drivers, mais les drivers ne doivent pas dependre de cette couche.

### 4. `Core/Inc/bsp` et `Core/Src/bsp`

Place ici l'initialisation specifique a la carte.

- creation de `pin1`, `pin2`, `pin3`
- configuration de l'ecran LCD
- regroupement des ressources liees a la carte

Cette couche est utile pour isoler ce qui changerait si la carte evolue, sans toucher a la logique applicative.

### 5. Fichiers CubeMX a ne pas restructurer

Les fichiers suivants doivent rester en place :

- `Core/Src/main.c`
- `Core/Inc/main.h`
- `Core/Src/gpio.c`, `i2c.c`, `spi.c`, `tim.c`, `usart.c`
- `Core/Inc/gpio.h`, `i2c.h`, `spi.h`, `tim.h`, `usart.h`
- `Core/Src/stm32f1xx_it.c`
- `Core/Src/stm32f1xx_hal_msp.c`

Le bon usage est de les transformer en points d'appel vers les modules applicatifs, pas de les deplacer.

## Migration des fichiers actuels

### Fichiers `GAUL_drivers`

Conserver le principe, mais rester dans `Core`.

Mapping recommande :

- `Core/Inc/GAUL_drivers/RFM22.h` -> `Core/Inc/gaul/drivers/rfm22.h`
- `Core/Src/GAUL_drivers/RFM22.c` -> `Core/Src/gaul/drivers/rfm22.c`
- `Core/Inc/GAUL_drivers/i2c_lcd.h` -> `Core/Inc/gaul/drivers/i2c_lcd.h`
- `Core/Src/GAUL_drivers/i2c_lcd.c` -> `Core/Src/gaul/drivers/i2c_lcd.c`
- `Core/Inc/GAUL_drivers/Pulse_pin.h` -> `Core/Inc/gaul/drivers/pulse_pin.h`
- `Core/Src/GAUL_drivers/Pulse_pin.c` -> `Core/Src/gaul/drivers/pulse_pin.c`
- `Core/Inc/GAUL_drivers/buzzer.h` -> `Core/Inc/gaul/drivers/buzzer.h`
- `Core/Src/GAUL_drivers/buzzer.c` -> `Core/Src/gaul/drivers/buzzer.c`
- `Core/Inc/GAUL_drivers/MLX90393.h` -> `Core/Inc/gaul/drivers/mlx90393.h`
- `Core/Src/GAUL_drivers/MLX90393.c` -> `Core/Src/gaul/drivers/mlx90393.c`
- `Core/Inc/GAUL_drivers/timer.h` -> `Core/Inc/gaul/drivers/timer_utils.h`
- `Core/Src/GAUL_drivers/timer.c` -> `Core/Src/gaul/drivers/timer_utils.c`
- `Core/Inc/GAUL_drivers/utility.h` -> a supprimer ou renommer selon son vrai role
- `Core/Src/GAUL_drivers/utility.c` -> a supprimer ou renommer selon son vrai role

### Fichiers `GAUL_utilitie`

Le dossier doit disparaitre en tant que fourre-tout.

Mapping recommande :

- `Core/Inc/GAUL_utilitie/gestion_lcd.h` -> `Core/Inc/app/services/ui_service.h`
- `Core/Src/GAUL_utilitie/gestion_lcd.c` -> `Core/Src/app/services/ui_service.c`
- `Core/Inc/GAUL_utilitie/var.h` -> `Core/Inc/app/app_state.h`
- `Core/Src/GAUL_utilitie/var.c` -> `Core/Src/app/app_state.c`
- `Core/Inc/GAUL_utilitie/utilitie.c` -> supprimer
- `Core/Src/GAUL_utilitie/utilitie.c` -> supprimer

## Decoupage recommande pour `main.c`

Le code actuel de `main.c` doit etre extrait en modules avec responsabilites simples.

### A deplacer vers `Core/Src/bsp/`

- `init_pin()` -> `Board_InitOutputs()`
- `init_lcd()` -> `Board_InitDisplay()`
- `init_global()` -> remplace par une initialisation explicite de l'etat et de la carte

### A deplacer vers `Core/Src/app/services/`

- `ensure_rx_mode()` -> `radio_service.c`
- `process_rfm22_interrupts()` -> `radio_service.c`
- `process_pushbuttons()` -> `input_service.c`
- `print_menu()` deja deplace dans `ui_service.c`

### A laisser dans `Core/Src/main.c`

- sequence de boot HAL/CubeMX
- creation du contexte applicatif
- appel de `App_Init()`
- boucle `while (1)` qui appelle `App_Run()`
- callback HAL tres legere qui redirige vers l'application si necessaire

Important : si `HAL_GPIO_EXTI_Callback()` reste dans `main.c`, elle doit seulement poser un flag ou deleguer a `App_OnExti()` dans une zone `USER CODE`.

## Exemple de responsabilites par module

### `app_state.[hc]`

Contient uniquement l'etat partage de l'application :

- flags ISR
- paquet courant
- RSSI courant et reference
- coordonnees GPS
- canal courant
- handles applicatifs si necessaire

Idealement, remplacer les variables globales `extern` par une structure unique :

```c
typedef struct
{
  volatile uint8_t rfm22_interrupt_flag;
  volatile uint8_t pushbutton_interrupt_flag;
  volatile uint8_t pushbutton_pushed[4];
  uint8_t packet[8];
  uint8_t rssi;
  uint8_t ref_rssi;
  uint32_t buzzer_freq;
  float latitude;
  float longitude;
  uint8_t channel;
  Pulse_Pin_Typedef led_rx;
  Pulse_Pin_Typedef led_warn;
  Pulse_Pin_Typedef led_fifo;
  I2C_LCD_HandleTypeDef lcd;
} AppState;
```

### `radio_service.[hc]`

Responsable de :

- initialiser le module RFM22 pour l'application
- verifier et maintenir le mode RX
- traiter les interruptions radio
- fournir les donnees utiles au reste de l'application

### `input_service.[hc]`

Responsable de :

- convertir les flags boutons en actions applicatives
- changer le canal
- memoriser le RSSI de reference

### `ui_service.[hc]`

Responsable de :

- formatage de l'affichage LCD
- barre de signal
- rendu de l'ecran principal

## Strategie specifique a CubeMX

### Ce qu'il faut faire

- garder `main.c` et `stm32f1xx_it.c` a leur place
- n'editer les fichiers generes que dans les blocs `USER CODE`
- ajouter les nouveaux modules dans des sous-dossiers de `Core/Inc` et `Core/Src`
- utiliser des includes du type `#include "app/app.h"` ou `#include "gaul/drivers/rfm22.h"`
- laisser CubeMX regenerer les init bas niveau sans toucher aux modules applicatifs

### Ce qu'il vaut mieux eviter

- deplacer les fichiers generes hors de `Core`
- renommer les fichiers generes par CubeMX
- mettre de la logique metier importante directement dans `MX_*_Init()` ou hors `USER CODE`
- multiplier les chemins d'include externes si ce n'est pas necessaire

## Conventions recommandees

- utiliser une seule convention de nommage, de preference en minuscules avec underscore pour les fichiers : `rfm22.c`, `pulse_pin.c`, `ui_service.c`
- garder les types en `PascalCase` ou suffixes existants si necessaire, mais etre constant
- aligner strictement la casse des includes avec le nom reel des dossiers
- supprimer les fichiers vides du depot
- eviter les variables globales exposees partout ; preferer une structure de contexte passee aux services

## Ordre de migration conseille

1. Renommer les includes pour corriger la casse et unifier les noms de fichiers.
2. Creer les sous-dossiers cibles dans `Core/Inc` et `Core/Src`.
3. Supprimer les fichiers vides dans `GAUL_utilitie`.
4. Extraire l'etat global de `var.[hc]` vers `app_state.[hc]`.
5. Extraire l'affichage LCD vers `ui_service.[hc]`.
6. Extraire la logique radio et boutons de `main.c` vers des services.
7. Reduire `main.c` a un bootstrap applicatif minimal dans ses zones `USER CODE`.
8. Eventuellement renommer ensuite `GAUL_drivers` et `GAUL_utilitie` vers les nouveaux sous-dossiers, sans sortir de `Core`.

## Resultat attendu

Au final :

- `Core` reste compatible CubeMX
- le code applicatif devient lisible et localisable
- les drivers deviennent reutilisables
- `main.c` reste a sa place mais ne porte plus la logique metier
- les futures evolutions de la carte ou de l'interface auront un impact plus localise