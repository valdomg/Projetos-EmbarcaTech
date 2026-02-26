import * as bcrypt from 'bcrypt';
import ApiError from '../../utils/errors.js';

class UserService {
  constructor(userModel) {
    this.userModel = userModel;
  }

  createUser = async (userData) => {
    if (!userData.name || !userData.email) {
      throw ApiError.badRequest("Nome e email são obrigatórios");
    }

    const existingUser = await this.userModel.findOne({ email: userData.email });
    if (existingUser) {
      throw ApiError.badRequest("Email já está em uso");
    }

    if (! await this.validatePassword(userData.password)) {
      throw ApiError.badRequest("Senha deve ter pelo menos 8 caracteres, incluindo uma letra maiúscula, uma letra minúscula, um número e um caractere especial");
    }
    userData.password = await this.hashPassword(userData.password);

    const user = new this.userModel(userData);
    await user.save();
    return { id: user._id, name: user.name, email: user.email, role: user.role };

  }

  getUserById = async (userId) => {
    if (!userId) {
      throw ApiError.badRequest("ID do usuário é obrigatorio");
    }

    const user = await this.userModel.findById(userId);
    if (!user) {
      throw ApiError.badRequest("Usuário não encontrado");
    }
    return user;
  }

  getAllUsers = async () => {
    const users = await this.userModel.find();
    return users.map(user => (
      {
        _id: user._id,
        name: user.name,
        email: user.email,
        role: user.role
      }
    ));
  }

  getUserByEmail = async (email) => {
    if (!email) {
      throw ApiError.badRequest("Email é obrigatório");
    }

    return this.userModel.findOne({ email });
  }

  updateUser = async (userId, updateData) => {
    if (updateData.email) {
      const existingUser = await this.userModel.findOne({ email: updateData.email });
      if (existingUser && existingUser.id !== userId) {
        throw ApiError.badRequest("Email já está em uso");
      }
    }

    const updatedUser = await this.userModel.findByIdAndUpdate(
      userId,
      { name: updateData.name, email: updateData.email, role: updateData.role },
      { new: true }
    );
    if (!updatedUser) {
      throw ApiError.notFound("Usuário não encontrado");
    }
    return { _id: updatedUser._id, name: updatedUser.name, email: updatedUser.email, role: updatedUser.role };
  }

  deleteUser = async (userId) => {
    if (!userId) {
      throw ApiError.badRequest("ID do usuário é obrigatorio");
    }
    const user = await this.userModel.findById(userId);
    if (!user) {
      throw ApiError.notFound("Usuário não encontrado");
    }

    await this.userModel.findByIdAndDelete(userId);
  }

  changePassword = async (userId, currentPassword, newPassword) => {
    if (!userId) {
      throw ApiError.badRequest("ID do usuário é obrigatorio");
    }
    if (! await this.validatePassword(newPassword)) {
      throw ApiError.badRequest("Senha deve ter pelo menos 8 caracteres, incluindo uma letra maiúscula, uma letra minúscula, um número e um caractere especial");
    }
    const user = await this.userModel.findById(userId).select('+password');
    if (!user) {
      throw ApiError.notFound("Usuário não encontrado");
    }
    const isMatch = await bcrypt.compare(currentPassword, user.password);
    if (!isMatch) {
      throw ApiError.unauthorized("Senha atual incorreta");
    }
    const hashedPassword = await this.hashPassword(newPassword);
    user.password = hashedPassword;
    await user.save();
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