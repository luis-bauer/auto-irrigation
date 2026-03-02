# Automated Irrigation System

> **Archived Project:** This is a school seminar project from 2023. The code is not being maintained anymore and serves as a reference for my first projects

![Image of the display](display.jpg)

## Overview
The goal of the seminar was to create a self-watering flowerbed. I was solely responsible for the entire software logic and the technical implementation of all features listed below.

## Functions
- ⏱️ **Auto-Interval-Irrigation:** If the soil moisture falls below the threshold, the water pump is switched on for a set interval and a cooldown of 30 minutes is started.
- 💧 **Manual-Irrigation:** Irrigation can be manually activated via the dashboard, regardless of humidity.
- 🛠️ **Dashboard-Display:** The connected display shows multiple windows that can be cycled through using a button. Among other things, it can display the last watering time, soil moisture, air humidity, and air temperature.

## Technologies used
- **Controller:** ESP8266
- **Sensors:** soil moisture (analog), DHT11 (temperature, air humidity)

## Reflection
From today's perspective, I would build the code more modularly and implement error handling for sensor timeouts. Nevertheless, it was a valuable project for learning the entire cycle from hardware planning to software deployment.



<details>
  <summary>🇩🇪 <b>Klicke hier für die deutsche Projektbeschreibung</b></summary>

# Automatisiertes Bewässerungsbeet

> **Archiviertes Projekt:** Dies ist mein P-Seminar Projekt aus dem Jahr 2023. Der Code wird nicht mehr gepflegt und dient als Referenz für meine ersten Projekte.

![Foto vom Beet](display.jpg)

## Überblick
Ziel des Seminars war die Erstellung eines selbstbewässernden Blumenfelds.
Dabei war ich zuständig für die Technik. Die folgenden Funktionen wurden vollständig von mir implementiert.

## Funktionen
- ⏱️ **Auto-Intervall-Bewässerung:** Unterschreitet die Feuchtigkeit den Grenzwert wird die Wasserpumpe für eine feste Zeit eingeschaltet und ein Cooldown von 30 Minuten wird gesetzt.
- 💧 **Manuelle-Bewässerung:** Über das Dashboard kann unabhängig vom Feuchtigkeitswert eine Bewässerung aktiviert werden.
- 🛠️ **Dashboard-Anzeige:** Das angeschlossene Display zeigt verschiedene Fenster zwischen denen mithilfe eines Knopfes durchgeschaltet werden kann. Unter anderem kann der letzte Bewässerungszeitpunkt, die Bodenfeuchtigkeit, die Luftfeuchtigkeit und die Lufttemperatur angezeigt werden.

## Verwendete Technologien
- **Controller:** ESP8266
- **Sensoren:** Bodenfeuchtigkeit (Analog), DHT11 (Temperatur, Luftfeuchtigkeit)

## Reflexion
Aus heutiger Sicht würde ich den Code modularer aufbauen und eine Fehlerbehandlung für Sensor-Timeouts implementieren. Dennoch war es ein wertvolles Projekt, um den gesamten Zyklus von Hardware-Planung bis Software-Deployment zu lernen.

</details>
