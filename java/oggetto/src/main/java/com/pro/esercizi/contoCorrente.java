package com.pro.esercizi;

public class contoCorrente {

  private String nome;
  private double saldo;

  public contoCorrente(String nome, double saldo) {
    this.nome = nome;
    this.saldo = saldo;

  }

  public String getNome() {
    return nome;
  }

  public double getSaldo() {
    return saldo;
  }

  public void setNome(String nome) {
    this.nome = nome;
  }

  public void setSaldo(double saldo) {
    this.saldo = saldo;
  }

  public void preleva(double prelievo) {
    saldo = saldo - prelievo;

  }

  public void deposita(double deposito) {
    saldo = saldo + deposito;
  }
}
