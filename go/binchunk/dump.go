package binchunk

import (
	"encoding/binary"
	"fmt"
	"math"
	"os"
)

type writer struct {
	data []byte
}

func (self *writer) writeByte(b byte) {
	// if b == 0 {
	// 	z := "0"
	// 	self.data = append(self.data, byte(z[0]))
	// }
	self.data = append(self.data, b)
}

func (self *writer) writeBytes(bytes []byte) {
	self.data = append(self.data, bytes...)
}

func (self *writer) writeUint32(i uint32) {
	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, i)
	self.data = append(self.data, buf...)
}

func (self *writer) writeUint64(i uint64) {
	buf := make([]byte, 8)
	binary.LittleEndian.PutUint64(buf, i)
	self.data = append(self.data, buf...)
}

func (self *writer) writeString(s string) {
	self.writeByte(byte(len(s) + 1)) // size (including the null terminator)
	self.writeBytes([]byte(s))
}

func (self *writer) writeLuaInteger(i int64) {
	self.writeUint64(uint64(i))
}

func (self *writer) writeLuaNumber(n float64) {
	self.writeUint64(math.Float64bits(n))
}
func Dump2Writer(proto *Prototype) *writer {

	w := &writer{}

	// Write the header
	w.writeBytes([]byte(AST_SIGNATURE)) // Lua signature
	w.writeByte(ASTC_VERSION)           // LUAC version
	w.writeByte(ASTC_FORMAT)            // LUAC format
	w.writeBytes([]byte(ASTC_DATA))     // LUAC data
	w.writeByte(CINT_SIZE)              // C int size
	w.writeByte(CSIZET_SIZE)            // C size_t size
	w.writeByte(INSTRUCTION_SIZE)       // instruction size
	w.writeByte(AST_INTEGER_SIZE)       // lua_Integer size
	w.writeByte(AST_NUMBER_SIZE)        // lua_Number size
	w.writeLuaInteger(ASTC_INT)         // LUAC integer
	w.writeLuaNumber(ASTC_NUM)          // LUAC number

	// Write the size of upvalues
	w.writeByte(2) // size_upvalues (this is just an example, you can adjust it based on actual usage)

	// Write the prototype
	writeProto(w, proto)
	return w
}
func Dump2Str(proto *Prototype) []byte {
	w := Dump2Writer(proto)
	return w.data
}
func Dump(proto *Prototype, filePath string) error {
	// Create a new writer to store the binary data

	w := Dump2Writer(proto)

	// Create or open the file for writing
	file, err := os.Create(filePath)
	if err != nil {
		return fmt.Errorf("unable to create file: %v", err)
	}
	defer file.Close()

	// Write the data to the file
	_, err = file.Write(w.data)
	if err != nil {
		return fmt.Errorf("unable to write data: %v", err)
	}

	return nil
}

func writeProto(w *writer, proto *Prototype) {
	// Write the source string
	w.writeString(proto.Source)

	// Write line information
	w.writeUint32(proto.LineDefined)
	w.writeUint32(proto.LastLineDefined)

	// Write the number of parameters, is vararg, and max stack size
	w.writeByte(proto.NumParams)
	w.writeByte(proto.IsVararg)
	w.writeByte(proto.MaxStackSize)

	// Write the code (bytecode)
	w.writeUint32(uint32(len(proto.Code)))
	for _, code := range proto.Code {
		w.writeUint32(code)
	}

	// Write the constants
	w.writeUint32(uint32(len(proto.Constants)))
	for _, constant := range proto.Constants {
		writeConstant(w, constant)
	}

	// Write the upvalues
	w.writeUint32(uint32(len(proto.Upvalues)))
	for _, upvalue := range proto.Upvalues {
		w.writeByte(upvalue.Instack)
		w.writeByte(upvalue.Idx)
	}

	// Write the prototypes (sub-functions)
	w.writeUint32(uint32(len(proto.Protos)))
	for _, subProto := range proto.Protos {
		writeProto(w, subProto)
	}

	// Write the line info
	w.writeUint32(uint32(len(proto.LineInfo)))
	for _, line := range proto.LineInfo {
		w.writeUint32(line)
	}

	// Write the local variables
	w.writeUint32(uint32(len(proto.LocVars)))
	for _, locVar := range proto.LocVars {
		w.writeString(locVar.VarName)
		w.writeUint32(locVar.StartPC)
		w.writeUint32(locVar.EndPC)
	}

	// Write the upvalue names
	w.writeUint32(uint32(len(proto.UpvalueNames)))
	for _, name := range proto.UpvalueNames {
		w.writeString(name)
	}
}

func writeConstant(w *writer, constant interface{}) {
	switch v := constant.(type) {
	case nil:
		w.writeByte(TAG_NIL)
	case bool:
		w.writeByte(TAG_BOOLEAN)
		if v {
			w.writeByte(1)
		} else {
			w.writeByte(0)
		}
	case int64:
		w.writeByte(TAG_INTEGER)
		w.writeLuaInteger(v)
	case float64:
		w.writeByte(TAG_NUMBER)
		w.writeLuaNumber(v)
	case string:
		w.writeByte(TAG_SHORT_STR) // assuming short string
		w.writeString(v)
	default:
		panic(fmt.Sprintf("unknown constant type: %T", v))
	}
}
