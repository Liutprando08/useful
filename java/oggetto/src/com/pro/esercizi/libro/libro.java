package com.pro.esercizi.libro;

public class libro {

  protected String libro;
  protected String autore;
  protected int anno;
  protected double prezzo;

  public libro(String libro, String autore, int anno, double prezzo) {
    this.libro = libro;
    this.autore = autore;
    this.anno = anno;
    this.prezzo = prezzo;
  }

  public String getLibro() {
    return libro;
  }

  public void setLibro(String libro) {
    this.libro = libro;
  }

  public String getAutore() {
    return autore;
  }

  public void setAutore(String autore) {
    this.autore = autore;
  }

  public int getAnno() {
    return anno;
  }

  public void setAnno(int anno) {
    this.anno = anno;
  }

  public double getPrezzo() {
    return prezzo;
  }

  public void setPrezzo(double prezzo) {
    this.prezzo = prezzo;
  }

  void modificaLibro(double prezzo) {
    this.prezzo = prezzo;
  }

  void modificaLibro(double prezzo, int anno) {
    this.prezzo = prezzo;
    this.anno = anno;
  }

  @Override
  public String toString() {
    return "libro [libro=" + libro + ", autore=" + autore + ", anno=" + anno + ", prezzo=" + prezzo + "]";
  }

}
