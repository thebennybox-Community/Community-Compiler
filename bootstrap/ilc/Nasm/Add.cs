namespace ilc.Nasm
{
    public class Add
    {
        public Registers Destination { get; set; }
        public object Source { get; set; }

        public Add(Registers destination, object source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"add {Destination}, {Source}";
        }
    }
}