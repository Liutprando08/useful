package com.pro.esercizi.biblioteca;

public class romanzo extends libro {
  protected String genere;

  public String getGenere() {
    return genere;
  }

  public void setGenere(String genere) {
    this.genere = genere;
  }

  public romanzo(String titolo, String autore, double prezzo, String genere) {
    super(titolo, autore, prezzo);
    this.genere = genere;
  }

}
