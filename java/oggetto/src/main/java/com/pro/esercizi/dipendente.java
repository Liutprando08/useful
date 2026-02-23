package com.pro.esercizi;

public abstract class dipendente {

  protected String nome;
  protected int matricola;
  protected double pagaBase;

  public dipendente(String nome, int matricola, double pagaBase) {
    this.nome = nome;
    this.matricola = matricola;
    this.pagaBase = pagaBase;
  }

  public abstract double stipendio();

  public int getMatricola() {
    return matricola;
  }

  public String getNome() {
    return nome;
  }

  public double getPagaBase() {
    return pagaBase;
  }

  public void setMatricola(int matricola) {
    this.matricola = matricola;
  }

  public void setNome(String nome) {
    this.nome = nome;
  }

  public void setPagaBase(double pagaBase) {
    this.pagaBase = pagaBase;
  }

  @Override
  public String toString() {
    return "dipendente [nome=" + nome + ", matricola=" + matricola + ", pagaBase=" + pagaBase + ", stipendio()="
        + stipendio() + "]";
  }

}
