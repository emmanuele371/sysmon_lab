# Protokoll: EPICS aSub-Record, SCAN-Jitter & CA-Overhead

## 1. Messdatenerfassung (Periodic SCAN)
Zur Auswertung des Systemverhaltens wurde der EPICS Soft-IOC unter macOS gestartet. Die Variablen `SYS:LOAD1` und `SYS:UPTIME` wurden mittels `camonitor` protokolliert und in die Datei `camonitor.log` geschrieben. Das SCAN-Feld des `aSub`-Records `SYS:STATS` war auf `"2 second"` konfiguriert. 

## 2. Messungsauswertung: Mittlere Periode
Aus der Logdatei lassen sich folgende exakte Zeitstempel fuer `SYS:UPTIME` entnehmen:
* Startzeitpunkt (erste Messung): 23:59:42.138941
* Endzeitpunkt (letzte Messung): 00:00:12.138763
* Vergangene Gesamtzeit (Delta T): 29.999822 Sekunden
* Anzahl der Messintervalle (N): 15

Die mittlere Periode (T_avg) berechnet sich wie folgt:
T_avg = Delta T / N = 29.999822 / 15 = 1.999988 Sekunden.

## 3. Diskussion der Abweichung vom Sollwert (Jitter)
Der ermittelte Mittelwert (1.999988 s) liegt extrem nah am Sollwert von 2.0 Sekunden. Bei Betrachtung der einzelnen Zeitstempel im Log fallen jedoch zeitliche Schwankungen (Jitter) im Millisekundenbereich auf (z.B. Intervall 1: 1.993 s, Intervall 2: 2.008 s). Diese Abweichungen sind systembedingt und haben zwei Hauptursachen:

1. **Scheduler-Jitter:** macOS ist ein General-Purpose-Betriebssystem und kein Echtzeitsystem (Hard RTOS). Der OS-Scheduler teilt die CPU-Zeit dynamisch auf. Wenn andere Hintergrundprozesse CPU-Ressourcen benoetigen, wird der EPICS-Thread minimal pausiert. Dies fuehrt dazu, dass der `aSub`-Record nicht exakt alle 2000 ms ausgefuehrt wird.
2. **CA-Overhead (Channel Access):** Nach der Auswertung durch den C-Code muessen die Daten ueber das Netzwerkprotokoll versendet werden. Die Generierung des Events und der Transport-Overhead fuegen eine zusaetzliche, variable Latenz hinzu, bevor `camonitor` den Wert samt Zeitstempel erfasst.

## 4. Bonus B1: SCAN Passive und caput PROC
Im zweiten Teil der Uebung wurde getestet, wie sich der IOC im Event-Driven-Modus verhaelt.
1. Umstellung auf Passive: `caput SYS:STATS.SCAN "Passive"`.
2. Beobachtung: Das automatische Polling stoppte sofort. Der `caget SYS:UPTIME` lieferte stets denselben (eingefrorenen) Wert.
3. Manueller Trigger: Mit dem Befehl `caput SYS:STATS.PROC 1` wurde der Record explizit zur Verarbeitung gezwungen. 
4. Ergebnis: Erst nach diesem manuellen Trigger wurde die C-Funktion ausgefuehrt, das OS neu ausgelesen und dank des `PP`-Flags (Process Passive) an den Ausgabe-Record `SYS:UPTIME` weitergeleitet, wodurch sich der Wert einmalig aktualisierte.
