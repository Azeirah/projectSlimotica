Om dit project te clonen:

Dit project maakt gebruik van submodules, contiki is een github repo. In deze repo zit een link naar deze contiki repo, dit heet een submodule. Als je nou dit project clonet, krijg je niet automatisch Contiki erbij, dus moet die ook gecloned worden, zie hieronder:

```
git clone --recursive https://github.com/Azeirah/projectSlimotica.git
```

Als je hem al hebt geclonet, en je wilt submodules downloaden (contiki is een submodule), gebruik dan:

```
git clone https://github.com/Azeirah/projectSlimotica.git
cd projectSlimotica
git submodule update --init --recursive
```

    ----------
46 |          | 1
   |          |
   |          |
   |   ----   |
   |  |    |  |
24 |   ----   | 23
    ----------

RS232 debugging zit op pinnen ZWART op GROUND, WIT (oranje in de RS232 kabel) op 26, en GEEL (geel in de RS232 kabel) op 28

Het blinkende ledje zit op pin 37.