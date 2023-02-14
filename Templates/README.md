# Testing your code

## Test Soort

Het testen van je code wordt gedaan doormiddel van GoogleTest.

### Klaarmaken om te testen

Om ervoor te zorgen dat je tests worden gedraaid, moet je je directory toevoegen aan het workflow.yml bestand.
Dit doe je door de onderstaande code te kopieren, en aanpassingen te maken waar komentaar bij staat.

#### PLATFORM IO

```yaml
NAME_HERE: # Dit moet je aanpassen
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v3

      - uses: actions/cache@v3
        with:
          path: |
            ~/.cache/pip
            ~/.platformio/.cache
          key: ${{ runner.os }}-pio

      - uses: actions/setup-python@v4
        with:
          python-version: '3.9'

      - name: Install PlatformIO Core
        run: pip install --upgrade platformio

      - name: Navigate
        run: cd ./ # Dit moet je aanpassen naar de pad van je PlatformIO directory (Vanuit de git root)

      - name: Run Unit Tests
        run: platformio test --environment native -f unit -v
```

Hierna moet je GoogleTest aan je PlatformIO project toevoegen. Dit kan je doen met de ingebouwde library manager.

## PlatformIO Testen

In het Unit Test Embedded Template is er een map aangemaakt genaamd `test`. Hierin kan je je eigen testen schrijven. Dit doe je in een .cpp bestand. Dit bestand kan je noemen zoals je wilt, mits je de naming conventie aanhoudt.

Onderaan Visual Studio Code staat een Erlenmeyer Flask. Als je hier op klikt, worden de tests uitgevoerd. Je krijgt een overzicht van welke tests er zijn uitgevoerd, en of ze zijn geslaagd of gefaald.

Testen worden ook automatish gedraaid door GitHub Actions wanneer je een merge request aanmaakt. De resultaten van je tests kan je vinden in je merge request.

## CMake Testen

Om je tests te draaien met CMake, moet je eerst de volgende stappen uitvoeren.

### Main CmakeLists.txt

Voeg het volgende toe aan de main CMakeLists.txt.

```cmake
add_subdirectory(test) # add test subdirectory
```

### Test CmakeLists.txt

Om er voor te zorgen dat je tests gevonden kunnen worden, moet je een CmakeLists.txt aanmaken in de test folder. Hierin moet je de volgende code toevoegen.

```cmake
set(Sources
    gtest_validation_test.cpp
)

add_executable(${ProjectTestName} ${Sources})

target_link_libraries(${ProjectTestName} PUBLIC
    gtest_main
    ${ProjectName}
)

add_test(
    NAME ${ProjectTestName}
    COMMAND ${ProjectTestName}
)
```

In de `Sources` variabele kan je de bestanden toevoegen die je wilt testen. Deze moeten wel in de test folder staan. Let op dat bestanden die hier niet in staan niet getest worden.

De rest zou je niet aan hoeven te passen. Als er iets niet werkt, contacteer een van de gitmasters.

### Uitvoeren van de tests

Net zoals de PlatformIO tests, worden deze automatisch uitgevoerd bij het mergen van een branch. Deze moeten allemaal slagen, anders wordt dit niet gemerged.

De tests kunnen ook lokaal worden uitgevoerd. Dit is makkelijk te doen via de CMake extension in Visual Studio Code. Er staat een Erlenmeyer Flask waar je op kan klikken om tests uit te voeren.

## Testen schrijven

### Plain tests

```cpp
TEST(test_BestandNaam, test_FunctieNaam_TestCase)
{
    EXPECT_EQ(1, 1); // Hier moet een test komen te staan.
}

// Een voorbeeld

TEST(test_Rekenmachine, test_Addition_Positives)
{
    EXPECT_EQ(Addition(1,1), 1); // Altijd Variabele, Expected
}

```

### Fixtures

Soms kan het zijn dat je voor meerdere tests telkens dezelfde stappen moet uitvoeren. Om dit makkelijker te maken kan je gebruik maken van `fixtures`. Dit zijn stukjes code die je kan draaien voor of na een test. GoogleTest heeft hier standaard manieren voor om dit te doen.

In de `SetUp()` functie kan je code zetten die voor elke test wordt uitgevoerd. In de `TearDown()` functie kan je code zetten die na elke test wordt uitgevoerd. Wat hier belangrijk is, is dat alle objecten die je **in** de functies aanmaakt niet toegankelijk zullen zijn in de test zelf. Als je objecten toegankelijk wil hebben in de tests zelf, moet je deze aanmaken als members van de class.

```cpp
class test_BestandNaam : public ::testing::Test
{
protected:
  int hierKanJeBij;
  virtual void SetUp()
  {
    int hierKanJeNietBij;
  }

  virtual void TearDown()
  { 
  }
};
```

Wat hier belangrijk is, is dat de class naam hetzelfde is als de suite naam.

Als je de fixture wilt gebruiken voor een test, moet de `TEST` vervangen door `TEST_F`. Hierdoor weet GoogleTest dat je een fixture gebruikt.

**Let er op dat de class naam hetzelfde is als de suite naam.**

```cpp
class TestGroupName : public ::testing::Test
{
protected:
  int a;
  virtual void SetUp()
  {
    a = 1;
  }

  virtual void TearDown()
  {
  }
};

// Deze test gebruikt de fixture TestGroupName
TEST_F(TestGroupName, TestName)
{
  ASSERT_EQ(a, 1);
}

// Dit zal niet werken, omdat 'a' niet toegankelijk is in deze test. Want deze test gebruikt geen fixture.
TEST(TestGroupName, TestName2)
{
  ASSERT_EQ(a, 1);
}
```
