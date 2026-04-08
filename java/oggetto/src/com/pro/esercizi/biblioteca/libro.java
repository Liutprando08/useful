package com.pro.esercizi.biblioteca;

public class libro {

  protected String titolo;
  protected String autore;
  protected double prezzo;

  public String getTitolo() {
    return titolo;
  }

  public void setTitolo(String titolo) {
    this.titolo = titolo;
  }

  public String getAutore() {
    return autore;
  }

  public void setAutore(String autore) {
    this.autore = autore;
  }

  public double getPrezzo() {
    return prezzo;
  }

  public void setPrezzo(double prezzo) {
    this.prezzo = prezzo;
  }

  public libro(String titolo, String autore, double prezzo) {
    this.titolo = titolo;
    this.autore = autore;
    this.prezzo = prezzo;
  }

  @Override
  public String toString() {
    return "libro [titolo=" + titolo + ", autore=" + autore + ", prezzo=" + prezzo + "]";
  }

}
