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
    userData.password = this.hashPassword(userData.password);

    const user = new this.userModel(userData);
    await user.save();
    return user;
  };

  validatePassword = async (password) => {
    const regex = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z0-9]).{8,}$/;
    return regex.test(password);
  }

  hashPassword = (plainPassword) => {
    const salt = bcrypt.genSaltSync(process.env.HASH_SALT_ROUNDS || 10);
    return bcrypt.hashSync(plainPassword, salt);
  }
}

export default UserService;