# Philogramm

![Color coding legend](/help/philogramm.png)

**Philogramm** is a visualizer for the 42 school project "Philosophers". It reads philosopher simulation output and displays a color-coded, easy-to-read chart of each philosopher's actions, including eating, sleeping, thinking, and death, as well as fork usage.

---

## Legend

![Color coding legend](/help/colorcoding.png)

Each bar shows a philosopher's actions over time, with colors indicating the action:

- **Eating**
- **Sleeping**
- **Thinking**
- **Dead**

---

## Fork Chart

```
	┌───────────────────────── Fork Chart ──────────────────────────┐
	│  Forks are shown as arrows between each philosopher           │
	│  pointing to the philosopher who picked them up.              │
	│                     ./philogramm -f                           │
	└───────────────────────────────────────────────────────────────┘
```

![Fork Example](/help/forkhelp.png)
![Fork Example](/help/forkchart.png)

⚠️  Important: Needs 'left' or 'right' in fork print message
```
601 5 has taken a fork right 
601 3 has taken left fork 
```

The location of "left" / "right" in the string section doesn't matter.

---

## Options

```
	Options
	┌───────────────┬────────────────────────────────────────────────┐
	│  <resolution> │ Scale the size of chart. Possible values:      │
	│               │ 5, 10, 20, 40 (default: 10)                    │
	│               │                                                │
	│  -i           │ Ignore actions of 3ms or less in chart output  │
	│  -f           │ Turn on the fork chart                         │
	│  -c           │ Turn off the eat count                         │
	│  -s           │ Turn off the scale                             │
	└───────────────┴────────────────────────────────────────────────┘
```

---



## Install

```bash
curl -o philogramm https://raw.githubusercontent.com/sugoidesune/42philogramm/master/philogramm
```
---

## Example Usage

```sh
./philo 6 400 200 200 5 | ./philogramm
./philo 6 400 200 200 | head -n 50 | ./philogramm
cat philo_output | ./philogramm -f
cat philo_output | ./philogramm 20 -i
./philo 6 400 200 200 | ./philogramm 20 -i -s -c -f
```
