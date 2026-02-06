package com.pro.esercizi;

public class cinema {

  private int capienzaMassima = 100;
  private int postiDisponibili;
  private String spettacolo;

  public cinema(String spettacolo, int postiPrenotazioneIniziale) {
    this.spettacolo = spettacolo;
    if (postiPrenotazioneIniziale > 100 || postiPrenotazioneIniziale < 1) {

    } else {
      postiDisponibili = 100 - postiPrenotazioneIniziale;
    }

  }

  public int getPostiDisponibili() {
    return postiDisponibili;
  }

  public boolean isSoldOut() {
    if (postiDisponibili <= 0) {
      return true;
    } else {
      return false;
    }
  }

  public Boolean prenota(int numeroBiglietti) {
    if (numeroBiglietti > 100 || numeroBiglietti < 1) {
      return false;
    } else {
      postiDisponibili = 100 - numeroBiglietti;
      return true;
    }

  }
}
