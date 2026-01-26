package com.pro.esercizi;

public class cane {
  private String nome;
  private String razza;
  private int eta;

  public void descrizioneCane() {
    System.out.println("nome: " + nome + "\n" + "razza: " + razza + "\n" + "eta: " + eta);

  }

  public void setNome(String nome) {
    if (nome.length() < 30) {
      this.nome = nome;
    }
  }

  public void setRazza(String razza) {
    this.razza = razza;
  }

  public void setEta(int eta) {
    this.eta = eta;
  }

  public int etaInAnniUmani(int eta) {
    int etaUmani = eta * 7;
    return etaUmani;
  }
}
