# Guida al Cooling Mod per Nintendo 3DS

Guida completa per migliorare il sistema di raffreddamento del Nintendo 3DS Original, ottimizzato per overclock e homebrew.

---

## Indice

1. [Prerequisiti](#prerequisiti)
2. [Fase 1: Thermal Paste Upgrade](#fase-1-thermal-paste-upgrade)
3. [Fase 2: Heatsink Installation](#fase-2-heatsink-installation)
4. [Fase 3: Fan Mod](#fase-3-fan-mod)
5. [Fase 4: Software Optimization](#fase-4-software-optimization)
6. [Fase 5: Verifica e Testing](#fase-5-verifica-e-testing)
7. [Risoluzione Problemi](#risoluzione-problemi)

---

## Prerequisiti

### Modello Consigliato
Questa guida è ottimizzata per **Nintendo 3DS Original** (prima generazione, 2011).

### Tool Necessari

| Tool | Descrizione |
|------|-------------|
| Cacciavite Tri-Wing | Per viti del case |
| Cacciavite Esagonale 4mm | Per viti interne |
| Cacciavite Esagonale 5mm | Per viti della scheda madre |
| Plettro in Plastica | Per aprire il case senza danni |
| Pinzette | Per componenti piccoli |
| Alcohol Isopropilico 99% | Per pulizia superfici |
| Cotton Fioc | Per applicazione pulizia |
| Pasta Termica | Vedi raccomandazioni sotto |
| Saldatore (opzionale) | Solo per Fan Mod |

### Materiali da Acquistare

| Componente | Dove Acquistare | Prezzo Indicativo |
|------------|-----------------|-------------------|
| Pasta Termica (Thermal Grizzly Kryonaut o Arctic MX-4) | Amazon, eBay | 5-15€ |
| Heatsink Rame 10x10x2mm | AliExpress | 2-5€ |
| Adesivo Termico (Thermal Tape) | AliExpress, Amazon | 3-8€ |
| Mini Ventola 5V 20x20mm (opzionale) | AliExpress, hobby shops | 3-8€ |
| Filo AWG 24-26 (opzionale) | negozi elettronica | 2-5€ |

### Preparazione
- [ ] Backup completo della SD card
- [ ] Verificare che Luma3DS sia aggiornato all'ultima versione
- [ ] Caricare la batteria sopra il 50%
- [ ] Fotografare il layout interno prima dello smontaggio

---

## Fase 1: Thermal Paste Upgrade

La pasta termica di fabbrica del 3DS è spesso di bassa qualità. Sostituirla può ridurre le temperature di **5-10°C**.

### Procedura

#### 1.1 Smontaggio
1. Spegnere completamente il 3DS
2. Rimuovere le viti Tri-Wing sul retro del case
3. Separare delicatamente le due metà del case con il plettro
4. Rimuovere le viti esagonali che fissano la scheda madre
5. Scollegare delicatamente i connettori della batteria e dei pulsanti

#### 1.2 Localizzazione CPU
- La CPU si trova al centro della scheda madre
- In alcuni modelli è sotto un piccolo shield metallico (da rimuovere con cautela)

#### 1.3 Pulizia
1. Applicare una piccola quantità di alcohol isopropilico su un cotton fioc
2. Rimuovere **completamente** la pasta termica esistente
3. Lasciare asciugare per 30 secondi

#### 1.4 Applicazione Pasta Termica
1. Posizionare una quantità grande quanto un chicco di riso al centro della CPU
2. Usare una scheda plastica (o spatola) per spalmare in uno strato sottile e uniforme
3. **Non esagerare**: uno strato troppo spesso peggiora la conduzione

### Pasta Termica Consigliata

| Prodotto | Conducibilità | Prezzo | Note |
|----------|---------------|--------|------|
| Thermal Grizzly Kryonaut | 12.5 W/mK | 10-15€ | Top di gamma |
| Arctic MX-4 | 8.5 W/mK | 5-8€ | Ottimo rapporto qualità/prezzo |
| Arctic Silver 5 | 8.9 W/mK | 5-8€ | Classico, richiede burn-in |
| Noctua NT-H1 | 4 W/mK | 8€ | Non conduttivo, sicuro |

---

## Fase 2: Heatsink Installation

Un heatsink in rame aumenta significativamente la superficie di dissipazione, garantendo un miglioramento di **10-15°C**.

### Materiali Necessari
- Heatsink in rame 10x10x2mm (o alluminio)
- Thermal tape 3M (spessore 0.5mm)
-oppure- thermal adhesive (per installazione permanente)

### Procedura

#### 2.1 Preparazione Heatsink
1. Pulire la superficie del heatsink con alcohol
2. Se si usa thermal tape: tagliare un quadrato leggermente più piccolo del heatsink
3. Se si usa adesivo termico: applicare uno strato sottile sulla superficie del heatsink

#### 2.2 Installazione
1. Posizionare il heatsink **al centro della CPU**
2. Premere delicatamente per 10-15 secondi
3. Verificare che sia ben aderente
4. Attendere 5 minuti prima di riassemblare

### Consigli
- Non far toccare il heatsink ai componenti vicini (cortocircuito)
- Se il heatsink è troppo grande, limarlo con carta vetrata fine
- Per overclock estremi: considerare un heatsink più grande (15x15mm)

---

## Fase 3: Fan Mod

Questa mod richiede saldature. Aggiunge raffreddamento attivo per overclock estremi.

### Attenzione
Questa modifica richiede competenze base di saldatura. Se non sei sicuro, procedi prima con le fasi 1 e 2.

### Componenti Necessari
- Mini ventola 5V 20x20mm o 30x30mm
- Filo conduttore AWG 24-26 (rosso e nero)
- Saldatore (30-40W)
- Stagno per saldatura
- Interruttore (opzionale ma consigliato)
- Condensatore 100µF (opzionale, per stabilizzare)

### Schema Collegamento

```
Battereria (+) ----> Ventola (+) ----> Interruttore ----> Battereria (+)
Battereria (-) ------------------------------------------> Ventola (-)
```

oppure usare i pin del connettore fan esistente (se presente sulla scheda):

```
Pin 1: +5V
Pin 2: GND
Pin 3: tach (opzionale, per rilevamento RPM)
```

### Procedura

#### 3.1 Identificazione Pin
- Cerca sulla scheda madre un connettore a 3 pin etichettato "FAN" o "FAN1"
- Se non presente: usare i pin della batteria

#### 3.2 Saldatura
1. Stagnare i fili (stagno su filo e su pin)
2. Saldare il filo rosso al pin +5V
3. Saldare il filo nero al pin GND
4. Se si usa interruttore: inserirlo nel filo positivo

#### 3.3 Installazione Ventola
1. Creare un supporto con nastro kapton o hot glue
2. Posizionare la ventola in modo che sofi verso il heatsink
3. Assicurarsi che le pale non tocchino nulla

#### 3.4 Test
1. Riaccendere il 3DS
2. Verificare che la ventola giri
3. Se non gira: controllare polarità

### Opzione Senza Saldatura
Per chi non vuole saldare:
- Usare un alimentatore USB esterno 5V
- Collegare la ventola a una power bank
- Non elegante ma funzionale

---

## Fase 4: Software Optimization

Ottimizzazioni software per ridurre temperature e consumi.

### Underclock tramite Luma3DS

1. Spegnere il 3DS
2. Tenere premuto SELECT
3. Accendere il 3DS
4. Navigare in "Luma3DS Configuration"
5. Cercare opzioni di frequenza CPU (se disponibili)

### Monitoraggio Temperature

Installare homebrew per monitorare le temperature:

1. Scaricare **Temperature Monitor** (temperature.3dsx)
2. Copiare nella cartella `/3ds/` sulla SD
3. Avviare Homebrew Launcher
4. Avviare Temperature Monitor
5. Verificare temperature durante uso normale

### Parametri Ottimali

| Scenario | Frequenza CPU | Temperatura Target |
|----------|---------------|-------------------|
| Uso normale | 268 MHz | < 40°C |
| Homebrew leggero | 400 MHz | < 45°C |
| Overclock medio | 500 MHz | < 50°C |
| Overclock estremo | 560 MHz | < 55°C |

### Limitazioni Overclock 3DS Original
Il 3DS originale ha limiti diversi rispetto al New 3DS:
- Frequenza base: **268 MHz**
- Frequenza massima: **~560 MHz** (dipende dal chip)
- Alcuni modelli non superano i 400 MHz stabilmente

---

## Fase 5: Verifica e Testing

### Stress Test Consigliati

1. **Boot Animation 3D**: Avviare un gioco 3D e lasciarlo in background
2. **Homebrew Emulatore**: Testare emulatori come PSP (PPSSPP non disponibile, ma GBA/SNES)
3. **Strumenti Benchmark**: Utilizzare homebrew di benchmark

### Checklist Post-Installazione

- [ ] Console si accende normalmente
- [ ] Schermo inferiore funziona
- [ ] Nessun artefatto grafico
- [ ] Temperatura stabile sotto 50°C
- [ ] Ventola silenziosa (se installata)
- [ ] Coverback si chiude correttamente

### Sintomi di Problemi

| Sintomo | Causa Probabile | Soluzione |
|---------|-----------------|-----------|
| Console si spegne | Surriscaldamento eccessivo | Verificare thermal paste |
| Artefatti grafici | CPU instabile | Ridurre frequenza |
| Boot loop | Cortocircuito | Controllare heatsink posizione |
| Calore eccessivo | Pasta termica mal applicata | Rimuovere e riapplicare |

---

## Risoluzione Problemi

### La temperatura non diminuisce
1. Verificare che la pasta termica sia applicata correttamente
2. Controllare che il heatsink sia a contatto con la CPU
3. Verificare che non ci sia polvere nel sistema

### La ventola non funziona
1. Controllare la polarità dei fili
2. Verificare la saldatura con multimetro
3. Provare con un'altra fonte di alimentazione

### Instabilità dopo overclock
1. Applicare più pasta termica
2. Installare heatsink più grande
3. Ridurre la frequenza di overclock
4. Aggiungere cooling attivo (ventola)

---

## Conclusione

Con questa guida dovresti essere in grado di:
- ✅ Ridurre le temperature di 15-25°C
- ✅ Stabilizzare overclock fino a 500-560MHz
- ✅ Prolungare la vita della console
- ✅ Migliorare le prestazioni per homebrew

### Combinazione Consigliata
Per risultati ottimali, combinare:
1. **Thermal Paste** (obbligatorio)
2. **Heatsink** (fortemente consigliato)
3. **Fan Mod** (per overclock estremi)

---

## Riferimenti

- [iFixit Nintendo 3DS Teardown](https://www.ifixit.com/Teardown/Nintendo-3DS-Teardown/5029)
- [GBAtemp 3DS Modding Forum](https://gbatemp.net/categories/nintendo-3ds.20/)
- [3DBrew Wiki](https://www.3dbrew.org/)

---

*Guida creata per scopi educativi. Eseguire modifiche hardware a proprio rischio.*
