package com.pro.esercizi.other;

public class ascensore {

  int pianoCorrente;
  boolean porteAperte;

  public ascensore(int pianoCorrente, boolean porteAperte) {
    this.pianoCorrente = pianoCorrente;
    this.porteAperte = porteAperte;
  }

  public int getPianoCorrente() {
    return pianoCorrente;
  }

  public void setPianoCorrente(int pianoCorrente) {
    this.pianoCorrente = pianoCorrente;
  }

  public boolean isPorteAperte() {
    return porteAperte;
  }

  public void setPorteAperte(boolean porteAperte) {
    this.porteAperte = porteAperte;
  }

  public void apriPorte() {
    porteAperte = true;
  }

  public void chiudiPorte() {
    porteAperte = false;
  }

  public void saliPiano() {
    pianoCorrente++;
  }

  public void scendiPiano() {
    pianoCorrente--;
  }

  public void scegliPiano(int piano) {
    pianoCorrente = piano;
  }

  @Override
  public String toString() {
    return "ascensore [pianoCorrente=" + pianoCorrente + ", porteAperte=" + porteAperte + "]";
  }

}
