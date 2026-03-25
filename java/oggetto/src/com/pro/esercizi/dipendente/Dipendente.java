public abstract class Dipendente {
    private String nome;
    private String cognome;
    private double stipendioBaseMensile;

    public Dipendente(String nome, String cognome, double stipendioBaseMensile) {
        this.nome = nome;
        this.cognome = cognome;
        this.stipendioBaseMensile = stipendioBaseMensile;
    }

    public String getNome() {
        return nome;
    }

    public String getCognome() {
        return cognome;
    }

    public double getStipendioBaseMensile() {
        return stipendioBaseMensile;
    }

    public abstract double calcolaStipendioAnnuo();

    @Override
    public String toString() {
        return "Dipendente{" +
                "nome='" + nome + '\'' +
                ", cognome='" + cognome + '\'' +
                ", stipendioBaseMensile=" + stipendioBaseMensile +
                ", stipendioAnnuo=" + calcolaStipendioAnnuo() +
                '}';
    }
}
