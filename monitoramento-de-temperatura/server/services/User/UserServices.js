import * as bcrypt from 'bcrypt';

class UserService {
  constructor(userModel) {
    this.userModel = userModel;
  }

  createUser = async (userData) => {
    if (!userData.name || !userData.email) {
      throw new Error("Nome e email são obrigatórios");
    }

    const existingUser = await this.userModel.findOne({ email: userData.email });
    if (existingUser) {
      throw new Error("Email já está em uso");
    }

    if(! await this.validatePassword(userData.password)) {
      throw new Error("Senha deve ter pelo menos 8 caracteres, incluindo uma letra maiúscula, uma letra minúscula, um número e um caractere especial");
    }
    userData.password = await this.hashPassword(userData.password);

    const user = new this.userModel(userData);
    await user.save();
    return user;

  }

  getUserById = async (userId) => {
    if(!userId) {
      throw new Error("ID do usuário é obrigatorio");
    }

    const user = await this.userModel.findById(userId);
    if(!user) {
      throw new Error("Usuário não encontrado");
    }
    return user;
  }

  getAllUsers = async () => {
    return this.userModel.find();
  }

  getUserByEmail = async (email) => {
    if (!email) {
      throw new Error("Email é obrigatório");
    }

    return this.userModel.findOne({ email });
  }

  updateUser = async (userId, updateData) => {
    if (updateData.email) {
      const existingUser = await this.userModel.findOne({ email: updateData.email });
      if (existingUser && existingUser.id !== userId) {
        throw new Error("Email já está em uso");
      }
    }

    const updatedUser = await this.userModel.findByIdAndUpdate(
      userId,
      { name: updateData.name, email: updateData.email },
      { new: true }
    );
    if (!updatedUser) {
      throw new Error("Usuário não encontrado");
    }
    return updatedUser;
  }

  deleteUser = async (userId) => {
    if(!userId) {
      throw new Error("ID do usuário é obrigatorio");
    }
    const user = await this.userModel.findById(userId);
    if(!user) {
      throw new Error("Usuário não encontrado");
    }

    await this.userModel.findByIdAndDelete(userId);
  }


  validatePassword = async (password) => {
    const regex = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z0-9]).{8,}$/;
    return regex.test(password);
  }

  hashPassword = async (plainPassword) => {
    const saltRounds = 10;
    const salt = await bcrypt.genSalt(saltRounds);
    return bcrypt.hash(plainPassword, salt);
  }
}

export default UserService;