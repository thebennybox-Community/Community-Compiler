namespace ilc.Nasm
{
    public class Mul
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Mul(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"mul {Destination}, {Source}";
        }
    }
}