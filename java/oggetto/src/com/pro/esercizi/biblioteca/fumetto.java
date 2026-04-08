package com.pro.esercizi.biblioteca;

public class fumetto extends libro {
  protected String illustratore;

  public fumetto(String titolo, String autore, double prezzo, String illustratore) {
    super(titolo, autore, prezzo);
    this.illustratore = illustratore;
  }

  public String getIllustratore() {
    return illustratore;
  }

  public void setIllustratore(String illustratore) {
    this.illustratore = illustratore;
  }

}
