package com.pro.esercizi.libro;

public class libroScolastico extends libro {
  protected String materia;

  public libroScolastico(String libro, String autore, int anno, double prezzo, String materia) {
    super(libro, autore, anno, prezzo);
    this.materia = materia;
  }

  public String getMateria() {
    return materia;
  }

  public void setMateria(String materia) {
    this.materia = materia;
  }

  @Override
  public String toString() {
    return "libroScolastico [libro=" + libro + ", materia=" + materia + ", autore=" + autore + ", anno=" + anno
        + ", prezzo=" + prezzo + "]";
  }

}
