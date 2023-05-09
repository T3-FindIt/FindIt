# Code Standaard

## Header Files

### C++

Deze dienen opgeslagen te worden als .hpp bestanden.

Deze moet ook multiple include bescherming te hebben.

```cpp
#ifndef HEADERFILENAME_HPP
#define HEADERFILENAME_HPP

#endif // HEADERFILENAME_HPP
```

Geen variablen buiten klasse definitie, tenzij ze constant zijn.

💻 - *Geautomatiseerd getest*

### C

Dezelfde regels gelden voor C bestanden, echter hoeven deze niet de .*pp extensie te hebben.

```c
#ifndef HEADERFILENAME_H
#define HEADERFILENAME_H

#endif // HEADERFILENAME_H
```

💻 - *Geautomatiseerd getest*

## Source Files

De volgende volgorde moet worden gehanteerd voor source bestanden.

- Includes
  - Eerst je eigen includes
  - Systeem includes
- Lokale constantes & defines
- Klasse
  - Constructor
  - Destructor
  - Publieke functies
  - Prive functies

```cpp
// Own Includes

// System Includes

// Local Defines

// Local Constants

// Class

    // Publieke Members

    // Prive Members

    // Constructor
    
    // Destructor

    // Publieke functies

    // Prive functies
```

Gebruik Class alleen als je CPP gebruikt

## Naming Convention

### Namen

| Symbol | Naming Convention | Example |
|---|---|---|
| Include Guard | ALLCAPS_H(PP) | #ifndef MYFILENAME_HPP |
| Class | PascalCase | class MotorExample |
| Functie | PascalCase | MotorExample::TurnOn(); |
| Variable | camelCase | int newInteger; |
| Constant | PascalCase | int DefaultSpeed = 100; |
| Define | ALL_CAPS | #define DEFAULT_SPEED 100 |
| Enum | PascalCase | enum SubStates |
| Enum Label | ALL_CAPS | SUBSTATE_IDLE |
| Interface | Moet starten met I | IPayout|
| Bestands Naam | PascalCase | MotorExample.cpp |

💻 - *Gedeeltjelijk geautomatiseerd getest*

### Extensies

| Bestandstype | Extensie |
|---|---|
| C++ Header | .hpp |
| C++ Source | .cpp |
| C Header | .h |
| C Source | .c |
| PlantUML | .wsd |

💻 - *Geautomatiseerd getest*

## Design

Er word geen code geschreven waarvan geen design bestaat. Private code hoeft niet perse in een design te staan, maar het wordt sterk aangeraden.

Er moet minstens een UML class diagram zijn.

Een uitzondering hierop zijn POCs.

## Code Stijl

## Krulhaken

Krulhaken moeten altijd starten op een nieuwe regel.

```cpp
while (true)
{
    if(this == other)
    {
        // Do something
    }
}
```

💻 - *Geautomatiseerd getest*

## One-Liners

❌ In verband met de vorige regel over krulhaken, zijn one-liners verboden. Het volgende is dus verboden:

```cpp
if (true) DoAThing();
```

✔️ Daarvoor zou in de plaats moeten komen:

```cpp
if (true)
{
    DoAThing();
}
```

❌ Dit geld ook voor Getter en Setter functies.

```cpp
public int GetSpeed() {return speed;}
```

💻 - *Geautomatiseerd getest*

## Whitespace

Er dient altijd Whitespace tussen/rondom bepaalde elementen te zitten:

- Na de komma (,)
- Voor en na binaire operaties (+ , - , & , == , etc.)
- Voor haakjes, tenzij je een functie aanroept
  - while ()
  - if ()
  - for ()
  - int speed = GetSpeed();

Tabs zijn 4 spaties breed.

💻 - *Geautomatiseerd getest*

## Goto, Continue & break

Deze mogen nooit worden gebruikt.

Voor break geld een uitzondering, mits het binnen een switch statement wordt gebruikt.

💻 - *Gedeeltjelijk geautomatiseerd getest*

## End of File Newline

Aan het einde van een bestand moet een newline staan. We maken gebruik van Github Actions om hierop te controlleren.

💻 - *Geautomatiseerd getest*

## Initializer lists (C++ specifiek)

Gebruik initializer lists wanner je geen checks hoeft te doen op de paramter en je deze niet veranderd in de methode.

```cpp
Car::Car(const int serialNumber,
         int nrDoors)
    : serialNumber(serialNumber)
    , nrDoors(nrDoors)
{
}
```

De parameters van de methode op een nieuwe regel moeten op dezelfde colom beginnen als waar de variabelen op de eerste regel beginnen.

De ':' van het begin van de initializer list moet op dezelfde colom staan als de 2e(!) ':' van de methode met class relatie. Hierna moet elke ',' ook op dezelfde colom staan.

### Volgorde

De volgorde van de intializer list moet hetzelfde zijn als de volgorde waarop de variabelen zijn gedeclareerd in de class.

## Pointer Parameters

Deze moeten altijd worden gecontroleerd op NULL, voordat deze wordt gedereferenced.

## Pointer to Memory after Free

Altijd een pointer op NULL zetten nadat geheugen is vrijgegeven, om eventuele memoryleaks te voorkomen

```cpp
 // some details
 delete animalPtr;
 animalPtr = NULL;
```

## Using std

```cpp
using namespace std;
```

Dit is nooit toegestaan.

💻 - *Geautomatiseerd getest*

## Afkortingen

Veel gebruikte (vak)termen zijn toegestaan om te gebruiken in code, alles wat hier niet aan voldoet moet worden uitgeschreven.

Als je een afkorting gebruikt, moet dit worden nagevraagd bij minstens 1 gitmaster. Is de afkorting eerder gebruikt, dan is is deze stap niet nodig.

**Voorbeelden** van veel gebruikte afkortingen:

- PWM
- USB
- GPIO
- NFC
