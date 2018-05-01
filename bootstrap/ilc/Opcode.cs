namespace ilc
{
    public class Opcode
    {
        public OpcodeType Id { get; set; }
        public object A0 { get; set; }
        public object A1 { get; set; }

        private bool EqualsString(Opcode z)
        {
            //stupid hax to fix broken shit
            
            var x = z.A0 as string;
            var y = A0 as string;

            var re = x.ToString() == y.ToString();

            return re;
        }

        public override bool Equals(object obj)
        {
            return Equals((Opcode)obj);
        }

        protected bool Equals(Opcode other)
        {
            return Id == other.Id && Equals(A0, other.A0) && Equals(A1, other.A1);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = (int) Id;
                hashCode = (hashCode * 397) ^ (A0 != null ? A0.GetHashCode() : 0);
                hashCode = (hashCode * 397) ^ (A1 != null ? A1.GetHashCode() : 0);
                return hashCode;
            }
        }
    }
}